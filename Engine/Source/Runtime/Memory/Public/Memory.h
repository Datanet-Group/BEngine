#pragma once
#include "BEngineMemoryAPI.h"
#include "Allocator.h"
#include "FrameArena.h"
#include "MemoryTags.h"
#include <cstddef>
#include <array>
#include <atomic>

namespace BMemory {

    struct BENGINE_MEMORY_API Stats {
        std::atomic<std::size_t> Current{0};
        std::atomic<std::size_t> Peak{0};
        std::atomic<std::size_t> TotalAllocated{0};
        std::atomic<std::size_t> TotalFreed{0};
        std::array<std::atomic<std::size_t>, static_cast<size_t>(EMemoryTag::COUNT)> ByTag{};
    };

    BENGINE_MEMORY_API void Init();          // setup default allocator, frame arena, etc.
    BENGINE_MEMORY_API void Shutdown();      // leak report
    BENGINE_MEMORY_API void BeginFrame();    // reset frame arena

    BENGINE_MEMORY_API IMalloc* GetAllocator();
    BENGINE_MEMORY_API void     SetAllocator(IMalloc* custom); // takes ownership

    BENGINE_MEMORY_API FrameArena& GetFrameArena();
    BENGINE_MEMORY_API Stats&      GetStats();

    // Tracking-friendly entry points used by NewDelete
    BENGINE_MEMORY_API void* Alloc(std::size_t size, std::size_t align, EMemoryTag tag);
    BENGINE_MEMORY_API void  Free (void* p);

} // namespace BMemory
