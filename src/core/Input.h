#pragma once

#include <SDL3/SDL.h>
#include <array>

// Класс для опроса состояния клавиатуры и мыши.
// Работает по принципу "текущий кадр vs предыдущий кадр",
// что позволяет отличать "клавиша зажата" от "клавиша только что нажата".
class Input {
public:
    Input();

    // Вызывается в НАЧАЛЕ каждого кадра, ДО PollEvents.
    // Копирует текущее состояние в предыдущее и сбрасывает
    // однокадровые флаги (нажатия/отпускания).
    void BeginFrame();

    // Вызывается во время PollEvents для обработки одного события.
    void ProcessEvent(const SDL_Event& event);

    // ===== Клавиатура =====

    // Клавиша зажата прямо сейчас
    bool IsKeyDown(SDL_Scancode key) const;

    // Клавиша была нажата именно в этом кадре (переход up -> down)
    bool IsKeyPressed(SDL_Scancode key) const;

    // Клавиша была отпущена именно в этом кадре (переход down -> up)
    bool IsKeyReleased(SDL_Scancode key) const;

    // ===== Мышь =====

    float GetMouseX() const { return m_mouseX; }
    float GetMouseY() const { return m_mouseY; }

    bool IsMouseButtonDown(Uint8 button) const;
    bool IsMouseButtonPressed(Uint8 button) const;
    bool IsMouseButtonReleased(Uint8 button) const;

private:
    // SDL scancode — это Uint8, максимальное значение 512,
    // но с запасом берём 512 (SDL_SCANCODE_COUNT в SDL3 = 512)
    static constexpr int KEY_COUNT    = 512;
    static constexpr int MOUSE_BUTTONS = 8;  // ЛКМ, ПКМ, СКМ + запас

    std::array<bool, KEY_COUNT>     m_currentKeys{};
    std::array<bool, KEY_COUNT>     m_previousKeys{};

    std::array<bool, MOUSE_BUTTONS> m_currentMouse{};
    std::array<bool, MOUSE_BUTTONS> m_previousMouse{};

    float m_mouseX = 0.0f;
    float m_mouseY = 0.0f;
};