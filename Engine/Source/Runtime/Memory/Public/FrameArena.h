#pragma once
#include "BEngineMemoryAPI.h"
#include <cstddef>
#include <vector>
#include <cstdint>

class BENGINE_MEMORY_API FrameArena {
public:
    explicit FrameArena(std::size_t defaultBlock = 1u<<20); // 1 MB
    ~FrameArena();

    void  Reset(); // keep blocks, just rewind
    void* Alloc(std::size_t size, std::size_t alignment);

private:
    struct Block {
        std::uint8_t* Base = nullptr;
        std::size_t   Cap = 0;
        std::size_t   Off = 0;
    };
    std::vector<Block> Blocks;
    std::size_t DefaultBlock = 0;

    void* AllocSlow(std::size_t size, std::size_t alignment);
};
