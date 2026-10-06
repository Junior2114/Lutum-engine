#include <SDL3/SDL_main.h>
#include "core/Application.h"

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;

    m2d::Application app;
    if (!app.Init("My 2D Engine", 1280, 720)) {
        return 1;
    }
    app.Run();
    return 0;
}