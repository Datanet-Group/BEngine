#pragma once
#include "BEngineCoreAPI.h"
#include "Object.h"
#include <array>
#include <vector>
#include <cstdint>
#include <SDL_scancode.h>

class Input;

// High-level game actions you can bind to keys.
// Extend as needed for other games (Pause, Screenshot, etc.).
enum class EAction : uint8_t {
    MoveLeft = 0,
    MoveRight,
    RotateCCW,
    RotateCW,
    SoftDrop,
    HardDrop,
    Reset,
    COUNT
};

// Simple action map that forwards to BCore::Input
class BENGINE_CORE_API InputMap : public BObject {
    BCLASS(InputMap)
public:
    explicit InputMap(Input* in = nullptr) : In(in) {}

    void SetInput(Input* in) { In = in; }

    // Bindings
    void Bind(EAction action, SDL_Scancode key);
    void UnbindAll(EAction action);
    void ClearAll();

    // Queries
    bool Pressed(EAction action) const; // just-pressed
    bool Down(EAction action) const;    // held

private:
    Input* In = nullptr;
    std::array<std::vector<SDL_Scancode>, static_cast<size_t>(EAction::COUNT)> Binds{};
};
