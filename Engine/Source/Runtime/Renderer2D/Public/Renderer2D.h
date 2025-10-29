#pragma once
#include "BEngineRenderer2DAPI.h"
#include <SDL.h>
#include <cstdint>

class BENGINE_RENDERER2D_API Renderer2D {
public:
    Renderer2D(SDL_Window* window, SDL_Renderer* renderer, int w, int h)
        : m_window(window), m_renderer(renderer), m_w(w), m_h(h) {}

    void Clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a=255);
    void Present();

    void FillRect(int x,int y,int w,int h,uint8_t r,uint8_t g,uint8_t b,uint8_t a=255);
    void DrawRect(int x,int y,int w,int h,uint8_t r,uint8_t g,uint8_t b,uint8_t a=255);

    int ScreenW() const { return m_w; }
    int ScreenH() const { return m_h; }

private:
    SDL_Window*   m_window   = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    int m_w=0, m_h=0;
};
