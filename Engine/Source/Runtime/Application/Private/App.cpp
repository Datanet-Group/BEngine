#include "App.h"
#include "IGame.h"
#include "Renderer2D.h"
#include "Input.h"
#include <SDL.h>
#include <iostream>

App::App(const AppConfig& c) : cfg(c) {}
App::~App() {
    if (game) { game->OnDetach(); game = nullptr; }
    delete r2d; delete input;
    if (sdlRenderer) SDL_DestroyRenderer(sdlRenderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

bool App::Init(IGame* g) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_AUDIO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n"; return false;
    }
    window = SDL_CreateWindow(cfg.title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              cfg.width, cfg.height, SDL_WINDOW_SHOWN);
    if (!window) { std::cerr << "CreateWindow failed: " << SDL_GetError() << "\n"; return false; }

    const int flags = cfg.vsync ? SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
                                : SDL_RENDERER_ACCELERATED;
    sdlRenderer = SDL_CreateRenderer(window, -1, flags);
    if (!sdlRenderer) { std::cerr << "CreateRenderer failed: " << SDL_GetError() << "\n"; return false; }

    int w,h; SDL_GetRendererOutputSize(sdlRenderer, &w, &h);
    r2d = new Renderer2D(window, sdlRenderer, w, h);
    input = new Input();

    game = g; game->OnAttach(r2d, input);
    running = true; return true;
}

void App::Run() {
    TimeStep ts; uint64_t freq = SDL_GetPerformanceFrequency();
    uint64_t prev = SDL_GetPerformanceCounter();

    while (running) {
        uint64_t now = SDL_GetPerformanceCounter();
        double frameDt = double(now - prev) / double(freq);
        prev = now; ts.accumulator += frameDt;

        SDL_Event e; input->BeginFrame();
        while (SDL_PollEvent(&e)) input->HandleEvent(e);
        input->EndFrame();

        int safety = 0;
        while (ts.accumulator >= ts.fixed && safety++ < 8) {
            UpdateContext ctx{ float(ts.fixed), float(frameDt), input->QuitRequested() };
            if (ctx.quitRequested) { running = false; break; }
            game->Update(ctx);
            ts.accumulator -= ts.fixed;
        }

        r2d->Clear(15,15,18,255);
        game->Render();
        r2d->Present();
    }
}
