#include "core/Application.h"

#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <string>

namespace m2d {

Application::Application() {}

Application::~Application() {
    Shutdown();
}

bool Application::Init(const char* title, int width, int height) {
    // ===== SDL =====
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    // ===== SDL_ttf =====
    if (!TTF_Init()) {
        std::cerr << "TTF_Init failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    // ===== Окно и рендерер =====
    // Флаг SDL_RENDERER_PRESENTVSYNC в SDL3 устарел — создаём без флагов,
    // а VSync включаем отдельным вызовом SDL_SetRenderVSync.
    if (!SDL_CreateWindowAndRenderer(title, width, height, 0,
                                     &m_window, &m_renderer)) {
        std::cerr << "Failed to create window/renderer: "
                  << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    // ===== VSync =====
    // 1 = синхронизация с каждым обновлением монитора (обычный VSync)
    if (!SDL_SetRenderVSync(m_renderer, 1)) {
        std::cerr << "Warning: VSync not supported: "
                  << SDL_GetError() << std::endl;
        // Не критично — продолжаем без VSync
    }

    // ===== Обёртка над рендерером =====
    m_rendererWrap = Renderer(m_renderer);

    // ===== Спрайт-лист игрока =====
    if (!m_playerSheet.LoadFromFile(m_renderer,
                                    "assets/textures/player_sheet.png")) {
        std::cerr << "Warning: player_sheet.png not loaded. "
                  << "Place it in assets/textures/" << std::endl;
    } else {
        std::cout << "Loaded player_sheet.png ("
                  << m_playerSheet.GetWidth() << "x"
                  << m_playerSheet.GetHeight() << ")" << std::endl;
    }

    // ===== Шрифт =====
    if (!m_font.LoadFromFile("assets/fonts/default.ttf", 24.0f)) {
        std::cerr << "Warning: default.ttf not loaded. "
                  << "Place it in assets/fonts/" << std::endl;
    } else {
        std::cout << "Loaded default.ttf" << std::endl;
    }

    // ===== Игрок =====
    m_player.Init(&m_playerSheet);

    // ===== Таймеры =====
    m_frequency   = (double)SDL_GetPerformanceFrequency();
    m_lastCounter = SDL_GetPerformanceCounter();
    m_running     = true;

    std::cout << "Engine initialized: " << title
              << " (" << width << "x" << height << ")" << std::endl;
    return true;
}

void Application::Run() {
    while (m_running) {
        // ===== Delta time =====
        Uint64 now = SDL_GetPerformanceCounter();
        float dt = (float)((now - m_lastCounter) / m_frequency);
        m_lastCounter = now;

        // Защита от экстремальных значений dt
        if (dt < 0.0f) dt = 0.0f;
        if (dt > 0.1f) dt = 0.1f;

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

        if (event.type == SDL_EVENT_QUIT) {
            m_running = false;
        }
    }
}

void Application::Update(float deltaTime) {
    // ===== FPS-счётчик =====
    m_fpsTimer   += deltaTime;
    m_frameCount += 1;

    if (m_fpsTimer >= 1.0f) {
        m_fps        = m_frameCount;
        m_frameCount = 0;
        m_fpsTimer  -= 1.0f;   // сохраняем остаток, не сбрасываем в 0
    }

    // ===== Выход по Esc =====
    if (m_input.IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
        m_running = false;
    }

    // ===== Логика игрока =====
    m_player.Update(m_input, deltaTime);

    // ===== Заголовок окна — отладка =====
    static float titleTimer = 0.0f;
    titleTimer += deltaTime;
    if (titleTimer >= 0.1f) {
        titleTimer = 0.0f;
        std::string title = "Player: " +
            std::to_string((int)m_player.GetX()) + ", " +
            std::to_string((int)m_player.GetY());
        SDL_SetWindowTitle(m_window, title.c_str());
    }
}

void Application::Render() {
    // ===== Очистка фона =====
    m_rendererWrap.Clear(30, 30, 30);

    // ===== Игрок =====
    m_player.Render(m_rendererWrap);

    // ===== FPS-счётчик в левом верхнем углу =====
    std::string fpsText = "FPS: " + std::to_string(m_fps);
    m_rendererWrap.DrawText(m_font, fpsText, 10.0f, 10.0f,
                            255, 255, 100, 255);   // жёлтый

    // ===== Показ кадра =====
    m_rendererWrap.Present();
}

void Application::Shutdown() {
    // m_playerSheet и m_font освободятся в своих деструкторах,
    // но можно вызвать Destroy() явно — не обязательно.

    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }

    // TTF_Quit должен быть вызван ПОСЛЕ освобождения всех шрифтов,
    // но у нас Font — член Application, и его деструктор вызовется
    // уже после Shutdown(). Поэтому явно освобождаем шрифт здесь.
    m_font.Destroy();

    TTF_Quit();
    SDL_Quit();

    m_frequency = 0.0;
    m_running   = false;
}

} // namespace m2d