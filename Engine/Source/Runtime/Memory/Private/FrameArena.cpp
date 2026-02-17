#include "FrameArena.h"
#include <cstdlib>
#include <algorithm>
#include <cstring>

static inline std::uintptr_t AlignUp(std::uintptr_t v, std::size_t a) {
    const std::size_t mask = a - 1;
    return (v + mask) & ~mask;
}

FrameArena::FrameArena(std::size_t defaultBlock) : DefaultBlock(defaultBlock) {
    Blocks.reserve(4);
}

FrameArena::~FrameArena() {
    for (auto& b : Blocks) std::free(b.Base);
}

void FrameArena::Reset() {
    for (auto& b : Blocks) b.Off = 0;
}

void* FrameArena::Alloc(std::size_t size, std::size_t alignment) {
    if (Blocks.empty()) {
        Block b{};
        b.Cap = std::max(DefaultBlock, size + alignment + 64);
        b.Base = static_cast<std::uint8_t*>(std::malloc(b.Cap));
        b.Off = 0;
        Blocks.push_back(b);
    }

    Block& cur = Blocks.back();
    std::uintptr_t base = reinterpret_cast<std::uintptr_t>(cur.Base);
    std::uintptr_t ptr  = AlignUp(base + cur.Off, std::max<std::size_t>(alignment, alignof(std::max_align_t)));
    std::size_t newOff  = (ptr - base) + size;
    if (newOff <= cur.Cap) {
        cur.Off = newOff;
        return reinterpret_cast<void*>(ptr);
    }
    return AllocSlow(size, alignment);
}

void* FrameArena::AllocSlow(std::size_t size, std::size_t alignment) {
    Block b{};
    b.Cap = std::max(DefaultBlock, size + alignment + 64);
    b.Base = static_cast<std::uint8_t*>(std::malloc(b.Cap));
    b.Off = 0;
    Blocks.push_back(b);
    return Alloc(size, alignment);
}
