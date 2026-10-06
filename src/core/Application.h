#pragma once

#include <SDL3/SDL.h>
#include "core/Input.h"
#include "graphics/Texture.h"
#include "graphics/Renderer.h"
#include "game/Player.h"

namespace m2d {

class Application {
public:
    Application();
    ~Application();

    bool Init(const char* title, int width, int height);
    void Run();
    void Shutdown();

private:
    void PollEvents();
    void Update(float deltaTime);
    void Render();

private:
    SDL_Window*   m_window   = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    bool          m_running  = false;

    Uint64 m_lastCounter = 0;
    double m_frequency   = 0.0;

    Input    m_input;
    Renderer m_rendererWrap;
    Texture  m_playerSheet;
    Player   m_player;
};

} // namespace m2d