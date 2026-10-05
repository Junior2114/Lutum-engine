#include "core/Application.h"

#include <iostream>

Application::Application() {
    // Всё в Init
}

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

    m_frequency    = (double)SDL_GetPerformanceFrequency();
    m_lastCounter  = SDL_GetPerformanceCounter();
    m_running      = true;

    std::cout << "Engine initialized: " << title
              << " (" << width << "x" << height << ")" << std::endl;
    return true;
}

void Application::Run() {
    while (m_running) {
        // 1. Считаем delta time
        Uint64 now = SDL_GetPerformanceCounter();
        float dt = (float)((now - m_lastCounter) / m_frequency);
        m_lastCounter = now;

        // 2. Обрабатываем события
        PollEvents();

        // 3. Обновляем логику
        Update(dt);

        // 4. Рисуем кадр
        Render();
    }
}

void Application::PollEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                m_running = false;
                break;

            // Сюда позже добавим обработку клавиш и мыши
            default:
                break;
        }
    }
}

void Application::Update(float deltaTime) {
    // Пока пусто. Здесь будут обновляться сущности, физика, анимации.
    (void)deltaTime; // чтобы компилятор не ругался на неиспользуемый параметр
}

void Application::Render() {
    // Очистка экрана тёмно-серым
    SDL_SetRenderDrawColor(m_renderer, 30, 30, 30, 255);
    SDL_RenderClear(m_renderer);

    // TODO: здесь будет отрисовка спрайтов, текста и т.д.

    // Показать кадр
    SDL_RenderPresent(m_renderer);
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