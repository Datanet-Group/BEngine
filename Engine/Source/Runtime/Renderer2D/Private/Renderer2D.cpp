#include "Renderer2D.h"

void Renderer2D::Clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(m_renderer, r,g,b,a);
    SDL_RenderClear(m_renderer);
}
void Renderer2D::Present() { SDL_RenderPresent(m_renderer); }
void Renderer2D::FillRect(int x,int y,int w,int h,uint8_t r,uint8_t g,uint8_t b,uint8_t a){
    SDL_Rect rc{ x,y,w,h }; SDL_SetRenderDrawColor(m_renderer,r,g,b,a); SDL_RenderFillRect(m_renderer,&rc);
}
void Renderer2D::DrawRect(int x,int y,int w,int h,uint8_t r,uint8_t g,uint8_t b,uint8_t a){
    SDL_Rect rc{ x,y,w,h }; SDL_SetRenderDrawColor(m_renderer,r,g,b,a); SDL_RenderDrawRect(m_renderer,&rc);
}
