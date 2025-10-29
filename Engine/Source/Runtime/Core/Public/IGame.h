#pragma once
#include <cstdint>

struct UpdateContext {
    float dt;
    float realDt;
    bool  quitRequested = false;
};

class Renderer2D;
class Input;

class IGame {
public:
    virtual ~IGame() = default;
    virtual void OnAttach(Renderer2D* renderer, Input* input) = 0;
    virtual void OnDetach() = 0;
    virtual void Update(const UpdateContext& ctx) = 0;
    virtual void Render() = 0;
};
