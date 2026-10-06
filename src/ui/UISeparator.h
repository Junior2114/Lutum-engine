#pragma once

#include "ui/UIElement.h"

namespace m2d {

// Горизонтальная линия-разделитель. Не кликабельна.
class UISeparator : public UIElement {
public:
    UISeparator() = default;

    void SetColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) {
        m_r = r; m_g = g; m_b = b; m_a = a;
    }

    void Render(Renderer& renderer) override;

private:
    Uint8 m_r = 55, m_g = 55, m_b = 75, m_a = 255;
};

} // namespace m2d