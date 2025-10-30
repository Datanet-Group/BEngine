#pragma once
#include "BEngineFrameworkAPI.h"
#include "Object.h"   // from BCore

class Renderer2D;

class BENGINE_FRAMEWORK_API BLevel : public BObject {
    BCLASS(BLevel)
public:
    virtual ~BLevel() = default;
    virtual bool Load()   { return true; }
    virtual void Unload() {}
    virtual void Tick(float) {}
    virtual void Render(Renderer2D&) {}
};
