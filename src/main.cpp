#include <SDL3/SDL_main.h>
#include "core/Engine.h"
#include "my_game/MyGame.h"

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;

    m2d::Engine engine;
    if (!engine.Init("My 2D Engine", 1280, 720)) {
        return 1;
    }

    mygame::MyGame game;
    engine.Run(game);

    return 0;
}