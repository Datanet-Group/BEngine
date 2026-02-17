#pragma once
#include <cstdint>

enum class EMemoryTag : uint32_t {
    Unknown = 0,
    Core,
    Renderer,
    Audio,
    Physics,
    AI,
    Network,
    UI,
    Game,
    Frame,     // frame/temporary
    COUNT
};
