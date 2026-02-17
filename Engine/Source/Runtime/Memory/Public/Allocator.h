#pragma once
#include "BEngineMemoryAPI.h"
#include "MemoryTags.h"
#include <cstddef>
#include <cstdint>

struct BENGINE_MEMORY_API AllocInfo {
    std::size_t Size = 0;
    std::size_t Alignment = alignof(std::max_align_t);
    EMemoryTag  Tag = EMemoryTag::Unknown;
};

struct BENGINE_MEMORY_API IMalloc {
    virtual ~IMalloc() = default;
    virtual void* Malloc (const AllocInfo& info) = 0;
    virtual void* Realloc(void* ptr, std::size_t newSize, std::size_t alignment) = 0;
    virtual void  Free   (void* ptr) = 0;
};
