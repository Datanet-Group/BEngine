#include "BWorld.h"
#include "BLevel.h"
#include "Renderer2D.h"
#include "Input.h"

void BWorld::OpenLevel(std::unique_ptr<BLevel> lvl) {
    if (Current) { Current->Unload(); }
    Current = std::move(lvl);
    if (Current) { Current->Load(); }
}

void BWorld::Tick(float dt) {
    if (Current) Current->Tick(dt);
}

void BWorld::Render() {
    if (Current && R) Current->Render(*R);
}
