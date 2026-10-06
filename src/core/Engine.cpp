#include "core/Engine.h"
#include "core/Game.h"

#include <SDL3_ttf/SDL_ttf.h>

namespace m2d {

Engine::Engine() {}

Engine::~Engine() {
    Shutdown();
}

bool Engine::Init(const char* title, int width, int height) {
    Log::Init("m2d.log");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        M2D_ERROR("SDL_Init failed: ", SDL_GetError());
        return false;
    }

    if (!TTF_Init()) {
        M2D_ERROR("TTF_Init failed: ", SDL_GetError());
        SDL_Quit();
        return false;
    }

    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d11");

    if (!SDL_CreateWindowAndRenderer(title, width, height, 0,
                                     &m_window, &m_renderer)) {
        M2D_ERROR("Failed to create window/renderer: ", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    // Включаем текстовый ввод — без этого SDL не присылает SDL_EVENT_TEXT_INPUT
    SDL_StartTextInput(m_window);

    if (!SDL_SetRenderVSync(m_renderer, 1)) {
        M2D_WARN("VSync not supported: ", SDL_GetError());
    }

    m_rendererWrap = Renderer(m_renderer);
    m_width  = width;
    m_height = height;

    m_frequency   = (double)SDL_GetPerformanceFrequency();
    m_lastCounter = SDL_GetPerformanceCounter();
    m_running     = true;

    // UI-шрифт
    auto* uiFont = m_resources.GetFont("assets/fonts/default.ttf", 18.0f);
    if (uiFont) {
        m_ui.Init(uiFont);
    }

    M2D_INFO("Engine initialized: ", title, " (", width, "x", height, ")");
    return true;
}

void Engine::Run(Game& game) {
    M2D_INFO("Calling game.OnInit...");
    game.OnInit(*this);
    M2D_INFO("Entering main loop");

    while (m_running) {
        Uint64 now = SDL_GetPerformanceCounter();
        float dt = (float)((now - m_lastCounter) / m_frequency);
        m_lastCounter = now;

        if (dt < 0.0f) dt = 0.0f;
        if (dt > 0.1f) dt = 0.1f;

        m_input.BeginFrame();
        PollEvents();

        m_debugOverlay.Update(dt);
        m_ui.Update(dt, m_input);

        if (m_input.IsKeyPressed(SDL_SCANCODE_F3)) {
            ToggleDebugOverlay();
        }

        game.OnUpdate(dt);
        game.OnRender(m_rendererWrap);

        m_ui.Render(m_rendererWrap);

        if (m_showDebugOverlay) {
            m_debugOverlay.Render(m_rendererWrap, m_width, m_height);
        }

        m_rendererWrap.Present();
    }

    M2D_INFO("Main loop ended");
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
    m_resources.Clear();

    if (m_window) {
        SDL_StopTextInput(m_window);
    }

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
    Log::Shutdown();
}

} // namespace m2d