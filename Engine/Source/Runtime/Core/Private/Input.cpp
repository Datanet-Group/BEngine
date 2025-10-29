
#include "Input.h"

void Input::BeginFrame() { prev = curr; }

void Input::HandleEvent(const SDL_Event& e) {
    if (e.type == SDL_QUIT) quit = true;
    if (e.type == SDL_KEYDOWN && !e.key.repeat) curr[e.key.keysym.scancode] = 1;
    if (e.type == SDL_KEYUP) curr[e.key.keysym.scancode] = 0;
}

void Input::EndFrame() {}

bool Input::KeyDown(SDL_Scancode sc) const { return curr[sc] != 0; }
bool Input::KeyPressed(SDL_Scancode sc) const { return curr[sc] && !prev[sc]; }
