#pragma once

#include <SDL3/SDL.h>
#include <array>
#include <string>

namespace m2d {

class Input {
public:
    Input();

    void BeginFrame();
    void ProcessEvent(const SDL_Event& event);

    // ===== Клавиатура =====
    bool IsKeyDown(SDL_Scancode key) const;
    bool IsKeyPressed(SDL_Scancode key) const;
    bool IsKeyReleased(SDL_Scancode key) const;

    // ===== Мышь =====
    float GetMouseX() const { return m_mouseX; }
    float GetMouseY() const { return m_mouseY; }

    bool IsMouseButtonDown(Uint8 button) const;
    bool IsMouseButtonPressed(Uint8 button) const;
    bool IsMouseButtonReleased(Uint8 button) const;

    // ===== Текстовый ввод (для UI-полей) =====
    // Символы, накопленные за этот кадр (из SDL_EVENT_TEXT_INPUT).
    const std::string& GetTextInput() const { return m_textInput; }

    // Enter и Backspace как отдельные флаги — они приходят не через TEXT_INPUT.
    bool WasEnterPressed() const     { return m_enterPressed; }
    bool WasBackspacePressed() const { return m_backspacePressed; }

private:
    static constexpr int KEY_COUNT     = 512;
    static constexpr int MOUSE_BUTTONS = 8;

    std::array<bool, KEY_COUNT>     m_currentKeys{};
    std::array<bool, KEY_COUNT>     m_previousKeys{};
    std::array<bool, MOUSE_BUTTONS> m_currentMouse{};
    std::array<bool, MOUSE_BUTTONS> m_previousMouse{};

    float m_mouseX = 0.0f;
    float m_mouseY = 0.0f;

    // Текстовый ввод за текущий кадр
    std::string m_textInput;
    bool        m_enterPressed     = false;
    bool        m_backspacePressed = false;
};

} // namespace m2d