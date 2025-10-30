#pragma once
#include "BLevel.h"

class Renderer2D;

class BENGINE_FRAMEWORK_API BLevel2D : public BLevel {
    BCLASS(BLevel2D)
public:
    BLevel2D(int width, int height, int tileSize)
        : W(width), H(height), Tile(tileSize) {}

    void SetBackground(uint8_t r, uint8_t g, uint8_t b, uint8_t a=255)
    { BgR=r; BgG=g; BgB=b; BgA=a; }

    int Width() const { return W; }
    int Height() const { return H; }
    int TileSize() const { return Tile; }

    void Render(Renderer2D& R) override;

private:
    int W=0, H=0, Tile=32;
    uint8_t BgR=15, BgG=15, BgB=18, BgA=255;
};
