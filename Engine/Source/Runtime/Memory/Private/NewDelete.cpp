#include "NewDelete.h"
#include "Memory.h"

// Exported helpers used by macros
void* BAlloc(std::size_t size, std::size_t align, EMemoryTag tag) {
    return BMemory::Alloc(size, align, tag);
}
void  BFree(void* p) {
    BMemory::Free(p);
}

#if BENGINE_DEFINE_GLOBAL_NEWDELETE

// Global operators route through BMemory with Unknown tag
void* operator new(std::size_t size) {
    return BMemory::Alloc(size, alignof(std::max_align_t), EMemoryTag::Unknown);
}
void* operator new[](std::size_t size) {
    return BMemory::Alloc(size, alignof(std::max_align_t), EMemoryTag::Unknown);
}
void* operator new  (std::size_t size, std::align_val_t align) {
    return BMemory::Alloc(size, static_cast<std::size_t>(align), EMemoryTag::Unknown);
}
void* operator new[](std::size_t size, std::align_val_t align) {
    return BMemory::Alloc(size, static_cast<std::size_t>(align), EMemoryTag::Unknown);
}

void operator delete(void* p) noexcept {
    BMemory::Free(p);
}
void operator delete[](void* p) noexcept {
    BMemory::Free(p);
}
void operator delete(void* p, std::size_t) noexcept {
    BMemory::Free(p);
}
void operator delete[](void* p, std::size_t) noexcept {
    BMemory::Free(p);
}
void operator delete(void* p, std::align_val_t) noexcept {
    BMemory::Free(p);
}
void operator delete[](void* p, std::align_val_t) noexcept {
    BMemory::Free(p);
}
#endif
