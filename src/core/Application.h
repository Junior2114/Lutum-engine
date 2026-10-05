#pragma once

#include <SDL3/SDL.h>
#include "core/Input.h"
#include "graphics/Texture.h"
#include "graphics/Renderer.h"

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
    Renderer m_rendererWrap;   // обёртка, НЕ владеет SDL_Renderer
    Texture  m_playerTexture;  // владеет текстурой игрока

    // Позиция игрока в мировых координатах
    float m_playerX = 100.0f;
    float m_playerY = 100.0f;

    // Скорость в пикселях в секунду
    static constexpr float PLAYER_SPEED = 300.0f;
};