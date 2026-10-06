#include "ui/UISlider.h"

namespace m2d {

void UISlider::StartDrag(float mx, float sliderWorldX) {
    m_dragging = true;
    UpdateDrag(mx, sliderWorldX);
}

void UISlider::UpdateDrag(float mx, float sliderWorldX) {
    if (!m_dragging) return;

    m_normalized = m_normalizedFromMouse(mx, sliderWorldX);

    const float newValue = m_min + m_normalized * (m_max - m_min);
    m_value = newValue;

    if (m_onChange) m_onChange(m_value);
}

void UISlider::EndDrag() {
    m_dragging = false;
}

float UISlider::m_normalizedFromMouse(float mx, float sliderWorldX) const {
    const float rel = (mx - sliderWorldX) / m_w;
    if (rel < 0.0f) return 0.0f;
    if (rel > 1.0f) return 1.0f;
    return rel;
}

void UISlider::Render(Renderer& renderer) {
    if (!m_visible) return;

    SDL_FRect track{ m_x, m_y + m_h * 0.5f - 4.0f, m_w, 8.0f };
    renderer.DrawRect(track, 40, 40, 50, 255);

    const float filledW = m_w * m_normalized;
    if (filledW > 0.0f) {
        SDL_FRect fill{ m_x, m_y + m_h * 0.5f - 4.0f, filledW, 8.0f };
        renderer.DrawRect(fill, 80, 140, 220, 255);
    }

    const float knobX = m_x + m_w * m_normalized;
    const float knobSize = 16.0f;
    SDL_FRect knob{
        knobX - knobSize * 0.5f,
        m_y + m_h * 0.5f - knobSize * 0.5f,
        knobSize,
        knobSize
    };

    const Uint8 k = (m_hovered || m_dragging) ? 255 : 220;
    renderer.DrawRect(knob, k, k, k, 255);
}

} // namespace m2d