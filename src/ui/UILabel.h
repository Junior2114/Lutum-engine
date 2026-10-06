#pragma once

#include <string>
#include "ui/UIElement.h"
#include "graphics/Font.h"

namespace m2d {

// Текстовая метка. Не кликабельна, только отображение.
class UILabel : public UIElement {
public:
    UILabel() = default;

    void SetFont(Font* font)      { m_font = font; }
    void SetText(const std::string& text) { m_text = text; }

    void SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) {
        m_r = r; m_g = g; m_b = b; m_a = a;
    }

    void Render(Renderer& renderer) override;

private:
    Font*       m_font = nullptr;
    std::string m_text;
    Uint8 m_r = 220, m_g = 220, m_b = 220, m_a = 255;
};

} // namespace m2d