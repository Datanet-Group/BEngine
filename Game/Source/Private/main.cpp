#include "Engine/Source/Runtime/Application/Public/App.h"
#include "TetrisGame.h"


#ifdef _WIN32
#include <SDL.h>
#endif


#ifdef main
#undef main
#endif

int SDL_main(int argc, char* argv[]) {
    AppConfig cfg;
    cfg.title = "Tetris";
    cfg.width = 1920;
    cfg.height = 1080;
    cfg.vsync = true;

    App app(cfg);
    TetrisGame game;

    if (!app.Init(&game)) {
        return -1;
    }

    app.Run();
    return 0;
}
