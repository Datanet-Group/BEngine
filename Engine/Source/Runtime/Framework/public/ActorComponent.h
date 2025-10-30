#pragma once
#include "BEngineFrameworkAPI.h"
#include "Object.h"

class AActor;

class BENGINE_FRAMEWORK_API UActorComponent : public BObject {
    BCLASS(UActorComponent)
public:
    virtual ~UActorComponent() = default;
    virtual void OnAttach(AActor* owner) { Owner = owner; }
    virtual void OnDetach() {}
    virtual void Tick(float /*dt*/) {}

protected:
    AActor* Owner = nullptr;
};
