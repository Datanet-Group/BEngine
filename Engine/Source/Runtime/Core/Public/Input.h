#pragma once
#include "BEngineCoreAPI.h"
#include <SDL.h>
#include <array>

class BENGINE_CORE_API Input {
public:
    void BeginFrame();
    void HandleEvent(const SDL_Event& e);
    void EndFrame();

    bool KeyDown(SDL_Scancode sc) const;
    bool KeyPressed(SDL_Scancode sc) const;
    bool QuitRequested() const { return quit; }

private:
    std::array<uint8_t, SDL_NUM_SCANCODES> curr{};
    std::array<uint8_t, SDL_NUM_SCANCODES> prev{};
    bool quit = false;
};
