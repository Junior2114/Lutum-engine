#pragma once

#include <string>
#include <functional>
#include "ui/UIElement.h"
#include "graphics/Font.h"

namespace m2d {

class UIButton : public UIElement {
public:
    UIButton() = default;

    // ===== Настройка =====
    void SetFont(Font* font)               { m_font = font; }
    void SetText(const std::string& text)  { m_text = text; }

    void SetColors(Uint8 bgR, Uint8 bgG, Uint8 bgB,
                   Uint8 hoverR, Uint8 hoverG, Uint8 hoverB,
                   Uint8 pressedR, Uint8 pressedG, Uint8 pressedB) {
        m_bgR = bgR; m_bgG = bgG; m_bgB = bgB;
        m_hoverR = hoverR; m_hoverG = hoverG; m_hoverB = hoverB;
        m_pressR = pressedR; m_pressG = pressedG; m_pressB = pressedB;
    }

    void SetTextColor(Uint8 r, Uint8 g, Uint8 b) {
        m_textR = r; m_textG = g; m_textB = b;
    }

    // ===== Обработчик клика =====
    void SetOnClick(std::function<void()> callback) {
        m_onClick = std::move(callback);
    }

    // ===== Состояние (управляется UILayer) =====
    void SetHovered(bool h) { m_hovered = h; }
    void SetPressed(bool p) { m_pressed = p; }
    bool IsHovered() const  { return m_hovered; }
    bool IsPressed() const  { return m_pressed; }

    // Вызывается из UILayer при клике
    void Click();

    // ===== Жизненный цикл =====
    void Render(Renderer& renderer) override;

private:
    Font*       m_font = nullptr;
    std::string m_text;

    Uint8 m_bgR    = 45, m_bgG    = 45, m_bgB    = 55;
    Uint8 m_hoverR = 70, m_hoverG = 70, m_hoverB = 90;
    Uint8 m_pressR = 30, m_pressG = 30, m_pressB = 40;

    Uint8 m_textR = 220, m_textG = 220, m_textB = 220;

    bool m_hovered = false;
    bool m_pressed = false;

    std::function<void()> m_onClick;
};

} // namespace m2d