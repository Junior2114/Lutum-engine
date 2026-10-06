#include "ui/UIButton.h"

namespace m2d {

void UIButton::Render(Renderer& renderer) {
    if (!m_visible) return;

    // ===== Фон =====
    const Uint8 r = m_hovered ? m_hoverR : m_bgR;
    const Uint8 g = m_hovered ? m_hoverG : m_bgG;
    const Uint8 b = m_hovered ? m_hoverB : m_bgB;

    SDL_FRect bg = GetRect();
    renderer.DrawRect(bg, r, g, b, 255);

    // ===== Текст по центру =====
    if (m_font && !m_text.empty()) {
        // Приблизительно центрируем текст: сдвигаем на ~10 пикселей от краёв.
        // Точное центрирование требует измерения ширины текста — сделаем позже.
        const float textX = m_x + 12.0f;
        const float textY = m_y + (m_h - 20.0f) * 0.5f;

        renderer.DrawString(*m_font, m_text, textX, textY,
                            m_textR, m_textG, m_textB, 255);
    }
}

} // namespace m2d