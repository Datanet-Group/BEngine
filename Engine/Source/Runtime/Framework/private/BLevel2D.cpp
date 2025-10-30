#include "BLevel2D.h"
#include "Renderer2D.h"

void BLevel2D::Render(Renderer2D& R) {
    const int pxW = W * Tile;
    const int pxH = H * Tile;
    const int ox  = (R.ScreenW() - pxW) / 2;
    const int oy  = (R.ScreenH() - pxH) / 2;

    // Background panel
    R.FillRect(ox-2, oy-2, pxW+4, pxH+4, BgR, BgG, BgB, BgA);

    // Optional: thin border
    R.DrawRect(ox-2, oy-2, pxW+4, pxH+4, 80, 80, 90, 255);
}
