#include "InputMap.h"
#include "Input.h"

void InputMap::Bind(EAction action, SDL_Scancode key) {
    auto& vec = Binds[static_cast<size_t>(action)];
    // avoid duplicates
    for (auto k : vec) if (k == key) return;
    vec.push_back(key);
}

void InputMap::UnbindAll(EAction action) {
    Binds[static_cast<size_t>(action)].clear();
}

void InputMap::ClearAll() {
    for (auto& v : Binds) v.clear();
}

bool InputMap::Pressed(EAction action) const {
    if (!In) return false;
    const auto& vec = Binds[static_cast<size_t>(action)];
    for (auto key : vec) if (In->KeyPressed(key)) return true;
    return false;
}

bool InputMap::Down(EAction action) const {
    if (!In) return false;
    const auto& vec = Binds[static_cast<size_t>(action)];
    for (auto key : vec) if (In->KeyDown(key)) return true;
    return false;
}
