#include "ui/UISlider.h"

namespace m2d {

void UISlider::StartDrag(float mouseX) {
    m_dragging = true;
    UpdateDrag(mouseX);
}

void UISlider::UpdateDrag(float mouseX) {
    if (!m_dragging) return;

    m_normalized = m_normalizedFromMouse(mouseX);

    const float newValue = m_min + m_normalized * (m_max - m_min);
    if (newValue != m_value) {
        m_value = newValue;
        if (m_onChange) m_onChange(m_value);
    }
}

void UISlider::EndDrag() {
    m_dragging = false;
}

float UISlider::m_normalizedFromMouse(float mouseX) const {
    const float rel = (mouseX - m_x) / m_w;
    if (rel < 0.0f) return 0.0f;
    if (rel > 1.0f) return 1.0f;
    return rel;
}

void UISlider::Render(Renderer& renderer) {
    if (!m_visible) return;

    // ===== Трек (фон) =====
    SDL_FRect track{ m_x, m_y + m_h * 0.5f - 4.0f, m_w, 8.0f };
    renderer.DrawRect(track, 40, 40, 50, 255);

    // ===== Заполненная часть =====
    const float filledW = m_w * m_normalized;
    if (filledW > 0.0f) {
        SDL_FRect fill{ m_x, m_y + m_h * 0.5f - 4.0f, filledW, 8.0f };
        renderer.DrawRect(fill, 80, 140, 220, 255);
    }

    // ===== Ползунок =====
    const float knobX = m_x + m_w * m_normalized;
    const float knobSize = 16.0f;
    SDL_FRect knob{
        knobX - knobSize * 0.5f,
        m_y + m_h * 0.5f - knobSize * 0.5f,
        knobSize,
        knobSize
    };

    const Uint8 kr = m_hovered || m_dragging ? 255 : 220;
    const Uint8 kg = m_hovered || m_dragging ? 255 : 220;
    const Uint8 kb = m_hovered || m_dragging ? 255 : 220;
    renderer.DrawRect(knob, kr, kg, kb, 255);
}

} // namespace m2d