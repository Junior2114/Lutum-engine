#include "core/Application.h"

#include <iostream>
#include <string>

Application::Application() {}

Application::~Application() {
    Shutdown();
}

bool Application::Init(const char* title, int width, int height) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!SDL_CreateWindowAndRenderer(title, width, height, 0,
                                     &m_window, &m_renderer)) {
        std::cerr << "Failed to create window/renderer: "
                  << SDL_GetError() << std::endl;
        return false;
    }

    // Оборачиваем SDL_Renderer в наш Renderer
    m_rendererWrap = Renderer(m_renderer);

    // Загружаем спрайт игрока
    if (!m_playerTexture.LoadFromFile(m_renderer,
                                      "assets/textures/player.png")) {
        std::cerr << "Warning: player.png not loaded. "
                  << "Place it in assets/textures/" << std::endl;
        // Не выходим — движок продолжит работать без спрайта
    } else {
        std::cout << "Loaded player.png ("
                  << m_playerTexture.GetWidth() << "x"
                  << m_playerTexture.GetHeight() << ")" << std::endl;
    }

    m_frequency   = (double)SDL_GetPerformanceFrequency();
    m_lastCounter = SDL_GetPerformanceCounter();
    m_running     = true;

    std::cout << "Engine initialized: " << title
              << " (" << width << "x" << height << ")" << std::endl;
    return true;
}

void Application::Run() {
    while (m_running) {
        Uint64 now = SDL_GetPerformanceCounter();
        float dt = (float)((now - m_lastCounter) / m_frequency);
        m_lastCounter = now;

        m_input.BeginFrame();
        PollEvents();
        Update(dt);
        Render();
    }
}

void Application::PollEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        m_input.ProcessEvent(event);
        switch (event.type) {
            case SDL_EVENT_QUIT:
                m_running = false;
                break;
            default:
                break;
        }
    }
}

void Application::Update(float deltaTime) {
    // Закрытие по Esc
    if (m_input.IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
        m_running = false;
    }

    // ===== Движение игрока =====
    // Скорость умножается на deltaTime — независимо от FPS
    float dx = 0.0f;
    float dy = 0.0f;

    if (m_input.IsKeyDown(SDL_SCANCODE_W) || m_input.IsKeyDown(SDL_SCANCODE_UP))
        dy -= 1.0f;
    if (m_input.IsKeyDown(SDL_SCANCODE_S) || m_input.IsKeyDown(SDL_SCANCODE_DOWN))
        dy += 1.0f;
    if (m_input.IsKeyDown(SDL_SCANCODE_A) || m_input.IsKeyDown(SDL_SCANCODE_LEFT))
        dx -= 1.0f;
    if (m_input.IsKeyDown(SDL_SCANCODE_D) || m_input.IsKeyDown(SDL_SCANCODE_RIGHT))
        dx += 1.0f;

    // Нормализация диагонали — чтобы по диагонали не двигаться быстрее
    if (dx != 0.0f && dy != 0.0f) {
        const float inv = 1.0f / 1.41421356f;  // 1 / sqrt(2)
        dx *= inv;
        dy *= inv;
    }

    m_playerX += dx * PLAYER_SPEED * deltaTime;
    m_playerY += dy * PLAYER_SPEED * deltaTime;

    // Обновляем заголовок окна — показываем позицию (отладка)
    static float timer = 0.0f;
    timer += deltaTime;
    if (timer >= 0.1f) {   // не чаще 10 раз в секунду
        timer = 0.0f;
        std::string title = "Player: " +
            std::to_string((int)m_playerX) + ", " +
            std::to_string((int)m_playerY);
        SDL_SetWindowTitle(m_window, title.c_str());
    }
}

void Application::Render() {
    // Очистка фона
    m_rendererWrap.Clear(30, 30, 30);

    // Рисуем игрока
    m_rendererWrap.DrawTexture(m_playerTexture, m_playerX, m_playerY);

    // Показываем кадр
    m_rendererWrap.Present();
}

void Application::Shutdown() {
    // m_playerTexture освободится автоматически в своём деструкторе
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    if (m_running || m_frequency > 0.0) {
        SDL_Quit();
        m_frequency = 0.0;
    }
    m_running = false;
}