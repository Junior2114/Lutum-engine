#include "core/Input.h"

namespace m2d {

Input::Input() {
    m_currentKeys.fill(false);
    m_previousKeys.fill(false);
    m_currentMouse.fill(false);
    m_previousMouse.fill(false);
}

void Input::BeginFrame() {
    // Копируем состояние прошлого кадра
    m_previousKeys  = m_currentKeys;
    m_previousMouse = m_currentMouse;

    // Сбрасываем накопленный текстовый ввод
    m_textInput.clear();
    m_enterPressed     = false;
    m_backspacePressed = false;
}

void Input::ProcessEvent(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_KEY_DOWN: {
            if (event.key.scancode < KEY_COUNT) {
                m_currentKeys[event.key.scancode] = true;
            }
            // Enter и Backspace — отдельные флаги для текстовых полей
            if (event.key.scancode == SDL_SCANCODE_RETURN) {
                m_enterPressed = true;
            }
            if (event.key.scancode == SDL_SCANCODE_BACKSPACE) {
                m_backspacePressed = true;
            }
            break;
        }
        case SDL_EVENT_KEY_UP: {
            if (event.key.scancode < KEY_COUNT) {
                m_currentKeys[event.key.scancode] = false;
            }
            break;
        }

        case SDL_EVENT_TEXT_INPUT: {
            // SDL присылает UTF-8 текст с учётом раскладки.
            // Для наших целей (цифры, точка) достаточно.
            m_textInput += event.text.text;
            break;
        }

        case SDL_EVENT_MOUSE_MOTION: {
            m_mouseX = event.motion.x;
            m_mouseY = event.motion.y;
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            if (event.button.button < MOUSE_BUTTONS) {
                m_currentMouse[event.button.button] = true;
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            if (event.button.button < MOUSE_BUTTONS) {
                m_currentMouse[event.button.button] = false;
            }
            break;
        }

        default:
            break;
    }
}

// ===== Клавиатура =====

bool Input::IsKeyDown(SDL_Scancode key) const {
    if (key < 0 || key >= KEY_COUNT) return false;
    return m_currentKeys[key];
}

bool Input::IsKeyPressed(SDL_Scancode key) const {
    if (key < 0 || key >= KEY_COUNT) return false;
    return m_currentKeys[key] && !m_previousKeys[key];
}

bool Input::IsKeyReleased(SDL_Scancode key) const {
    if (key < 0 || key >= KEY_COUNT) return false;
    return !m_currentKeys[key] && m_previousKeys[key];
}

// ===== Мышь =====

bool Input::IsMouseButtonDown(Uint8 button) const {
    if (button >= MOUSE_BUTTONS) return false;
    return m_currentMouse[button];
}

bool Input::IsMouseButtonPressed(Uint8 button) const {
    if (button >= MOUSE_BUTTONS) return false;
    return m_currentMouse[button] && !m_previousMouse[button];
}

bool Input::IsMouseButtonReleased(Uint8 button) const {
    if (button >= MOUSE_BUTTONS) return false;
    return !m_currentMouse[button] && m_previousMouse[button];
}

} // namespace m2d