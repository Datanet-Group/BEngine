#pragma once
#include "BEngineApplicationAPI.h"
#include "BEngineCoreAPI.h"
#include "Time.h"

struct SDL_Window;
struct SDL_Renderer;
class IGame;
class Renderer2D;
class Input;

struct AppConfig {
    const char* title = "BEngine 2D";
    int width  = 480;
    int height = 960;
    bool vsync = true;
};

class BENGINE_APPLICATION_API App {
public:
    App(const AppConfig& cfg);
    ~App();

    bool Init(IGame* gameInstance); // game owned by caller
    void Run();

private:
    AppConfig cfg;
    SDL_Window* window = nullptr;
    SDL_Renderer* sdlRenderer = nullptr;
    Renderer2D* r2d = nullptr;
    Input* input = nullptr;
    IGame* game = nullptr;
    bool running = false;
};
