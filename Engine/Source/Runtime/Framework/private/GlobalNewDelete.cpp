#include "Memory.h"
#include <new>
#include <cstddef>

void* operator new(std::size_t size) {
    return BMemory::Alloc(size, alignof(std::max_align_t), EMemoryTag::Core);
}
void* operator new[](std::size_t size) {
    return BMemory::Alloc(size, alignof(std::max_align_t), EMemoryTag::Core);
}
void* operator new  (std::size_t size, std::align_val_t al) {
    return BMemory::Alloc(size, static_cast<std::size_t>(al), EMemoryTag::Core);
}
void* operator new[](std::size_t size, std::align_val_t al) {
    return BMemory::Alloc(size, static_cast<std::size_t>(al), EMemoryTag::Core);
}

static inline void delete_impl(void* p) noexcept {
    if (!p) return;
    BMemory::Free(p);
}

void operator delete(void* p) noexcept { delete_impl(p); }
void operator delete[](void* p) noexcept { delete_impl(p); }
void operator delete(void* p, std::size_t) noexcept { delete_impl(p); }
void operator delete[](void* p, std::size_t) noexcept { delete_impl(p); }
void operator delete(void* p, std::align_val_t) noexcept { delete_impl(p); }
void operator delete[](void* p, std::align_val_t) noexcept { delete_impl(p); }
