#include "Memory.h"
#include <new>
#include <cstddef>

void* operator new(std::size_t size) {
    return BMemory::Alloc(size, alignof(std::max_align_t), EMemoryTag::Game);
}
void* operator new[](std::size_t size) {
    return BMemory::Alloc(size, alignof(std::max_align_t), EMemoryTag::Game);
}
void* operator new  (std::size_t size, std::align_val_t al) {
    return BMemory::Alloc(size, static_cast<std::size_t>(al), EMemoryTag::Game);
}
void* operator new[](std::size_t size, std::align_val_t al) {
    return BMemory::Alloc(size, static_cast<std::size_t>(al), EMemoryTag::Game);
}

void operator delete(void* p) noexcept { BMemory::Free(p); }
void operator delete[](void* p) noexcept { BMemory::Free(p); }
void operator delete(void* p, std::size_t) noexcept { BMemory::Free(p); }
void operator delete[](void* p, std::size_t) noexcept { BMemory::Free(p); }
void operator delete(void* p, std::align_val_t) noexcept { BMemory::Free(p); }
void operator delete[](void* p, std::align_val_t) noexcept { BMemory::Free(p); }
