#pragma once

#include <SDL3/SDL.h>
#include "core/Input.h"
#include "graphics/Texture.h"
#include "graphics/Renderer.h"
#include "graphics/Animator.h"

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

    void SetupAnimations();   // <-- новое: настройка анимаций

private:
    SDL_Window*   m_window   = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    bool          m_running  = false;

    Uint64 m_lastCounter = 0;
    double m_frequency   = 0.0;

    Input    m_input;
    Renderer m_rendererWrap;
    Texture  m_playerSheet;   // <-- теперь спрайт-лист, не одиночный спрайт
    Animator m_animator;      // <-- новое

    float m_playerX = 100.0f;
    float m_playerY = 100.0f;

    static constexpr float PLAYER_SPEED = 200.0f;
    static constexpr int   FRAME_SIZE   = 32;   // размер кадра в спрайт-листе
};