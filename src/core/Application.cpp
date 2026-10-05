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

    m_frequency   = (double)SDL_GetPerformanceFrequency();
    m_lastCounter = SDL_GetPerformanceCounter();
    m_running     = true;

    std::cout << "Engine initialized: " << title
              << " (" << width << "x" << height << ")" << std::endl;
    return true;
}

void Application::Run() {
    while (m_running) {
        // 1. Delta time
        Uint64 now = SDL_GetPerformanceCounter();
        float dt = (float)((now - m_lastCounter) / m_frequency);
        m_lastCounter = now;

        // 2. Начало кадра — фиксируем предыдущее состояние ввода
        m_input.BeginFrame();

        // 3. События
        PollEvents();

        // 4. Логика
        Update(dt);

        // 5. Рендер
        Render();
    }
}

void Application::PollEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // Сначала отдаём событие в Input — он обновит своё состояние
        m_input.ProcessEvent(event);

        // Потом обрабатываем системные события
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
    (void)deltaTime;

    // ===== ТЕСТ СИСТЕМЫ ВВОДА =====

    // Закрытие по Esc
    if (m_input.IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
        std::cout << "Esc pressed — closing." << std::endl;
        m_running = false;
    }

    // Space — меняем заголовок окна (раз консоль не видна в WIN32-сборке)
    if (m_input.IsKeyPressed(SDL_SCANCODE_SPACE)) {
        static int counter = 0;
        ++counter;
        std::string title = "Space pressed: " + std::to_string(counter) + " times";
        SDL_SetWindowTitle(m_window, title.c_str());
    }

    // Проверка IsKeyDown — двигаем цвет фона, пока зажата стрелка вправо
    if (m_input.IsKeyDown(SDL_SCANCODE_RIGHT)) {
        // Просто демонстрация: меняем цвет очистки экрана
        // (реально это будет в Render, но для теста — здесь)
    }
}

void Application::Render() {
    // Меняем цвет фона в зависимости от зажатой клавиши — визуальная проверка
    if (m_input.IsKeyDown(SDL_SCANCODE_RIGHT)) {
        SDL_SetRenderDrawColor(m_renderer, 60, 30, 30, 255);   // красноватый
    } else if (m_input.IsKeyDown(SDL_SCANCODE_LEFT)) {
        SDL_SetRenderDrawColor(m_renderer, 30, 30, 60, 255);   // синеватый
    } else {
        SDL_SetRenderDrawColor(m_renderer, 30, 30, 30, 255);   // тёмно-серый
    }

    SDL_RenderClear(m_renderer);

    // TODO: отрисовка спрайтов

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