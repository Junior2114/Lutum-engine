#pragma once

#include <SDL3/SDL.h>
#include <array>

namespace m2d {

class Input {
public:
    Input();

    void BeginFrame();
    void ProcessEvent(const SDL_Event& event);

    bool IsKeyDown(SDL_Scancode key) const;
    bool IsKeyPressed(SDL_Scancode key) const;
    bool IsKeyReleased(SDL_Scancode key) const;

    float GetMouseX() const { return m_mouseX; }
    float GetMouseY() const { return m_mouseY; }

    bool IsMouseButtonDown(Uint8 button) const;
    bool IsMouseButtonPressed(Uint8 button) const;
    bool IsMouseButtonReleased(Uint8 button) const;

private:
    static constexpr int KEY_COUNT     = 512;
    static constexpr int MOUSE_BUTTONS = 8;

    std::array<bool, KEY_COUNT>     m_currentKeys{};
    std::array<bool, KEY_COUNT>     m_previousKeys{};
    std::array<bool, MOUSE_BUTTONS> m_currentMouse{};
    std::array<bool, MOUSE_BUTTONS> m_previousMouse{};

    float m_mouseX = 0.0f;
    float m_mouseY = 0.0f;
};

} // namespace m2d