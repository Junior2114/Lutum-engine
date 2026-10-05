#pragma once

#include <SDL3/SDL.h>

class Application {
public:
    Application();
    ~Application();

    // Инициализация: окно, рендерер
    bool Init(const char* title, int width, int height);

    // Главный цикл — крутится, пока пользователь не закроет окно
    void Run();

    // Освобождение ресурсов
    void Shutdown();

private:
    // Обработка всех событий SDL за текущий кадр
    void PollEvents();

    // Обновление логики (пока пусто, но место зарезервировано)
    void Update(float deltaTime);

    // Отрисовка кадра
    void Render();

private:
    SDL_Window*   m_window   = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    bool          m_running  = false;

    // Для расчёта delta time
    Uint64 m_lastCounter = 0;
    double m_frequency   = 0.0;
};