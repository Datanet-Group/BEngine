#pragma once
#include "BLevel.h"
#include "BEngineFrameworkAPI.h"
#include "Object.h"
#include <memory>

class Renderer2D;
class Input;
class BLevel;

class BENGINE_FRAMEWORK_API BWorld : public BObject {
    BCLASS(BWorld)
public:
    void SetIO(Renderer2D* r, Input* i) { R=r; I=i; }

    void OpenLevel(std::unique_ptr<BLevel> lvl);
    BLevel* GetCurrentLevel() const { return Current.get(); }

    Renderer2D* GetRenderer() const { return R; }
    Input*      GetInput()    const { return I; }

    void Tick(float dt);
    void Render();

private:
    Renderer2D* R = nullptr;
    Input*      I = nullptr;
    std::unique_ptr<BLevel> Current;
};
