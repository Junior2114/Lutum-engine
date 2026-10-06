#include "ui/UIButton.h"
#include "core/Log.h"

namespace m2d {

void UIButton::Click() {
    M2D_INFO("Button '", m_text, "' clicked");
    if (m_onClick) {
        m_onClick();
    } else {
        M2D_WARN("Button '", m_text, "' has no onClick handler");
    }
}

void UIButton::Render(Renderer& renderer) {
    if (!m_visible) return;

    Uint8 r, g, b;
    if (m_pressed) {
        r = m_pressR; g = m_pressG; b = m_pressB;
    } else if (m_hovered) {
        r = m_hoverR; g = m_hoverG; b = m_hoverB;
    } else {
        r = m_bgR; g = m_bgG; b = m_bgB;
    }

    SDL_FRect bg = GetRect();
    renderer.DrawRect(bg, r, g, b, 255);

    if (m_font && !m_text.empty()) {
        const float textX = m_x + 12.0f;
        const float textY = m_y + (m_h - 20.0f) * 0.5f;
        renderer.DrawString(*m_font, m_text, textX, textY,
                            m_textR, m_textG, m_textB, 255);
    }
}

} // namespace m2d