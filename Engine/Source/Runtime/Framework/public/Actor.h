#pragma once
#include "BEngineFrameworkAPI.h"
#include "Object.h"
#include <vector>

class Renderer2D;
class UActorComponent;
class UWorld;

class BENGINE_FRAMEWORK_API AActor : public BObject {
    BCLASS(AActor)
public:
    virtual ~AActor();

    virtual void BeginPlay() {}
    virtual void Tick(float /*dt*/) {}
    virtual void Render(Renderer2D& /*R*/) {}

    void   SetWorld(UWorld* w) { World = w; }
    UWorld* GetWorld() const   { return World; }

    void SetPosition(int x, int y) { X = x; Y = y; }
    int  GetX
