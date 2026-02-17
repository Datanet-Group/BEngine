#include "DefaultAllocator.h"
#include <cstdlib>

#if defined(_WIN32)
  #include <malloc.h> // _aligned_malloc/_aligned_free
#endif

namespace {
    struct DefaultMalloc final : IMalloc {
        void* Malloc(const AllocInfo& info) override {
            const std::size_t align = info.Alignment ? info.Alignment : alignof(std::max_align_t);
#if defined(_WIN32)
            return _aligned_malloc(info.Size, align);
#else
            void* p = nullptr;
            std::size_t a = align < sizeof(void*) ? sizeof(void*) : align;
            if (posix_memalign(&p, a, info.Size) != 0) return nullptr;
            return p;
#endif
        }
        void* Realloc(void* ptr, std::size_t newSize, std::size_t alignment) override {
            // Simple, safe path: alloc new, free old. Caller should copy if needed.
            void* np = Malloc(AllocInfo{ newSize, alignment, EMemoryTag::Unknown });
#if defined(_WIN32)
            // no memcpy here; size unknown to allocator layer by design
            if (ptr) Free(ptr);
#else
            if (ptr) Free(ptr);
#endif
            return np;
        }
        void Free(void* ptr) override {
#if defined(_WIN32)
            _aligned_free(ptr);
#else
            std::free(ptr);
#endif
        }
    };
}

IMalloc* CreateDefaultAllocator() {
    static DefaultMalloc* inst = new DefaultMalloc();
    return inst;
}
