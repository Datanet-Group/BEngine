#include "Memory.h"
#include "Allocator.h"
#include "DefaultAllocator.h"
#include <new>
#include <cstring>
#include <cstdint>
#include <algorithm> // std::max

namespace {

// Engine-tracking header marker
constexpr std::uint32_t MAGIC = 0xBEEFABCD;

struct Header {
    std::uint32_t Magic;
    std::uint32_t Tag;
    std::size_t   Size;
    std::size_t   Align;
    void*         Raw;   // pointer returned by underlying allocator
};

// --- Make all global singletons "never destruct" (heap-backed) ---

BMemory::Stats& GStats() {
    static auto* S = new BMemory::Stats();
    return *S;
}

IMalloc*& GMalloc() {
    static auto* M = new IMalloc*(nullptr);
    return *M;
}

FrameArena*& GArena() {
    static auto* A = new FrameArena*(nullptr);
    return *A;
}

IMalloc*& GDefault() {
    static auto* D = new IMalloc*(nullptr);
    return *D;
}

struct MallocHolder {
    IMalloc* Impl = nullptr; // owned custom allocator
};
MallocHolder& Holder() {
    static auto* H = new MallocHolder();
    return *H; // never destruct
}

// Ensure allocator pointers exist even if Alloc/Free are called before Init().
static inline void EnsureAllocatorReady() {
    if (!GMalloc()) {
        if (!GDefault()) GDefault() = CreateDefaultAllocator();
        GMalloc() = GDefault();
    }
}

} // namespace

namespace BMemory {

void Init() {
    if (!GDefault()) GDefault() = CreateDefaultAllocator();
    if (!GMalloc())  GMalloc()  = GDefault();
    if (!GArena())   GArena()   = new FrameArena(1u << 20); // 1 MB blocks

    // clear per-tag counters
    for (auto& a : GetStats().ByTag) a.store(0);
}

void Shutdown() {
    // Keep everything alive at process exit to avoid destructor-order issues.
    if (GArena()) GArena()->Reset();

    // If you ever set a custom allocator, keep it alive too (intentional leak).
    // Holder().Impl remains allocated; OS will reclaim on process exit.
}

void BeginFrame() {
    if (GArena()) GArena()->Reset();
}

IMalloc* GetAllocator() { return GMalloc(); }

void SetAllocator(IMalloc* custom) {
    // Replace current runtime allocator; keep previous alive to avoid teardown-order problems.
    Holder().Impl = custom;
    GMalloc() = custom ? Holder().Impl : GDefault();
}

FrameArena& GetFrameArena() { return *GArena(); }
Stats&      GetStats()      { return GStats(); }

void* Alloc(std::size_t size, std::size_t align, EMemoryTag tag) {
    EnsureAllocatorReady();

#if BENGINE_MEM_TRACK
    align = std::max<std::size_t>(align, alignof(std::max_align_t));

    const std::size_t hdr   = sizeof(Header);
    const std::size_t total = hdr + size + align; // extra to align payload

    AllocInfo info;
    info.Size      = total;
    info.Alignment = align;
    info.Tag       = tag;

    void* raw = GetAllocator()->Malloc(info);
    if (!raw) throw std::bad_alloc{};

    std::uintptr_t base    = reinterpret_cast<std::uintptr_t>(raw) + hdr;
    std::uintptr_t aligned = (base + (align - 1)) & ~(static_cast<std::uintptr_t>(align) - 1);

    auto* h = reinterpret_cast<Header*>(aligned - hdr);
    h->Magic = MAGIC;
    h->Tag   = static_cast<std::uint32_t>(tag);
    h->Size  = size;
    h->Align = align;
    h->Raw   = raw;

    auto& st = GetStats();
    st.TotalAllocated.fetch_add(size);
    auto cur = st.Current.fetch_add(size) + size;
    if (cur > st.Peak.load()) st.Peak.store(cur);
    st.ByTag[static_cast<size_t>(tag)].fetch_add(size);

    return reinterpret_cast<void*>(aligned);
#else
    AllocInfo info{ size, align, tag };
    return GetAllocator()->Malloc(info);
#endif
}

// Returns true if 'p' had our header and was freed via BMemory.
static inline bool TryFreeTrackedInternal(void* p) {
#if BENGINE_MEM_TRACK
    if (!p) return true;

    auto* h = reinterpret_cast<Header*>(
        reinterpret_cast<std::uintptr_t>(p) - sizeof(Header)
    );

    if (h->Magic != MAGIC) return false;

    auto& st = GetStats();
    const auto tag = static_cast<EMemoryTag>(h->Tag);
    st.TotalFreed.fetch_add(h->Size);
    st.Current.fetch_sub(h->Size);
    st.ByTag[static_cast<size_t>(tag)].fetch_sub(h->Size);

    GetAllocator()->Free(h->Raw);
    return true;
#else
    (void)p;
    return false;
#endif
}

bool TryFreeTracked(void* p) {
    EnsureAllocatorReady();
    return TryFreeTrackedInternal(p);
}

void Free(void* p) {
    EnsureAllocatorReady();

#if BENGINE_MEM_TRACK
    if (!TryFreeTrackedInternal(p)) {
        // Not tracked by BMemory; intentionally do nothing here.
        // (Global delete operators in modules will fall back to std::free.)
    }
#else
    if (p) GetAllocator()->Free(p);
#endif
}

} // namespace BMemory
