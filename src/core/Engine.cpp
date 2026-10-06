#include "core/Engine.h"
#include "core/Game.h"

#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>

namespace m2d {

Engine::Engine() {}

Engine::~Engine() {
    Shutdown();
}

bool Engine::Init(const char* title, int width, int height) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!TTF_Init()) {
        std::cerr << "TTF_Init failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d11");

    if (!SDL_CreateWindowAndRenderer(title, width, height, 0,
                                     &m_window, &m_renderer)) {
        std::cerr << "Failed to create window/renderer: "
                  << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    if (!SDL_SetRenderVSync(m_renderer, 1)) {
        std::cerr << "Warning: VSync not supported: "
                  << SDL_GetError() << std::endl;
    }

    m_rendererWrap = Renderer(m_renderer);
    m_width  = width;
    m_height = height;

    m_frequency   = (double)SDL_GetPerformanceFrequency();
    m_lastCounter = SDL_GetPerformanceCounter();
    m_running     = true;

    std::cout << "[Engine] Initialized: " << title
              << " (" << width << "x" << height << ")" << std::endl;
    return true;
}

void Engine::Run(Game& game) {
    game.OnInit(*this);

    while (m_running) {
        Uint64 now = SDL_GetPerformanceCounter();
        float dt = (float)((now - m_lastCounter) / m_frequency);
        m_lastCounter = now;

        if (dt < 0.0f) dt = 0.0f;
        if (dt > 0.1f) dt = 0.1f;

        m_input.BeginFrame();
        PollEvents();

        m_debugOverlay.Update(dt);

        if (m_input.IsKeyPressed(SDL_SCANCODE_F3)) {
            ToggleDebugOverlay();
        }

        game.OnUpdate(dt);
        game.OnRender(m_rendererWrap);

        if (m_showDebugOverlay) {
            m_debugOverlay.Render(m_rendererWrap, m_width, m_height);
        }

        m_rendererWrap.Present();
    }

    game.OnShutdown();
}

void Engine::PollEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        m_input.ProcessEvent(event);
        if (event.type == SDL_EVENT_QUIT) {
            m_running = false;
        }
    }
}

void Engine::Shutdown() {
    // Сначала ресурсы — они владеют текстурами и шрифтами
    m_resources.Clear();

    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }

    if (m_frequency > 0.0) {
        TTF_Quit();
        SDL_Quit();
        m_frequency = 0.0;
    }

    m_running = false;
}

} // namespace m2d