#include "core/Application.h"

#include <iostream>
#include <string>

namespace m2d {

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

    m_rendererWrap = Renderer(m_renderer);

    if (!m_playerSheet.LoadFromFile(m_renderer,
                                    "assets/textures/player_sheet.png")) {
        std::cerr << "Warning: player_sheet.png not loaded." << std::endl;
    } else {
        std::cout << "Loaded player_sheet.png ("
                  << m_playerSheet.GetWidth() << "x"
                  << m_playerSheet.GetHeight() << ")" << std::endl;
    }

    m_player.Init(&m_playerSheet);

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

        // Защита от экстремальных значений dt
        if (dt < 0.0f)   dt = 0.0f;
        if (dt > 0.1f)   dt = 0.1f;

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
    if (m_input.IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
        m_running = false;
    }

    m_player.Update(m_input, deltaTime);

    // Заголовок окна — отладка
    static float timer = 0.0f;
    timer += deltaTime;
    if (timer >= 0.1f) {
        timer = 0.0f;
        std::string title = "Player: " +
            std::to_string((int)m_player.GetX()) + ", " +
            std::to_string((int)m_player.GetY());
        SDL_SetWindowTitle(m_window, title.c_str());
    }
}

void Application::Render() {
    m_rendererWrap.Clear(30, 30, 30);
    m_player.Render(m_rendererWrap);
    m_rendererWrap.Present();
}

void Application::Shutdown() {
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

} // namespace m2d