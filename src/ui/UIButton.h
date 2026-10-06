#pragma once

#include <string>
#include <functional>
#include "ui/UIElement.h"
#include "graphics/Font.h"

namespace m2d {

// Кнопка: прямоугольник с текстом. Пока без клика — только визуал.
// Клик добавим следующим шагом.
class UIButton : public UIElement {
public:
    UIButton() = default;

    void SetFont(Font* font)      { m_font = font; }
    void SetText(const std::string& text) { m_text = text; }

    void SetColors(Uint8 bgR, Uint8 bgG, Uint8 bgB,
                   Uint8 hoverR, Uint8 hoverG, Uint8 hoverB) {
        m_bgR = bgR; m_bgG = bgG; m_bgB = bgB;
        m_hoverR = hoverR; m_hoverG = hoverG; m_hoverB = hoverB;
    }

    void SetTextColor(Uint8 r, Uint8 g, Uint8 b) {
        m_textR = r; m_textG = g; m_textB = b;
    }

    // ===== Состояние =====
    void SetHovered(bool h) { m_hovered = h; }
    bool IsHovered() const  { return m_hovered; }

    void Render(Renderer& renderer) override;

private:
    Font*       m_font = nullptr;
    std::string m_text;

    Uint8 m_bgR = 45, m_bgG = 45, m_bgB = 55;
    Uint8 m_hoverR = 70, m_hoverG = 70, m_hoverB = 90;

    Uint8 m_textR = 220, m_textG = 220, m_textB = 220;

    bool m_hovered = false;
};

} // namespace m2d