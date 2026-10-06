#pragma once

#include <functional>
#include "ui/UIElement.h"

namespace m2d {

class UISlider : public UIElement {
public:
    UISlider() = default;

    void SetRange(float minV, float maxV) {
        m_min = minV; m_max = maxV;
    }

    // Установить значение снаружи.
    // НЕ меняет состояние, если сейчас тащат.
    void SetValue(float value) {
        if (m_dragging) return;

        if (value < m_min) value = m_min;
        if (value > m_max) value = m_max;

        m_value = value;
        m_normalized = (m_max > m_min)
                       ? (m_value - m_min) / (m_max - m_min)
                       : 0.0f;
    }

    float GetValue() const { return m_value; }
    float GetNormalized() const { return m_normalized; }

    void SetOnChange(std::function<void(float)> callback) {
        m_onChange = std::move(callback);
    }

    void SetHovered(bool h) { m_hovered = h; }
    bool IsHovered() const  { return m_hovered; }

    // Начать drag. mx — абсолютная координата мыши.
    // sliderWorldX — абсолютная координата левого края слайдера.
    void StartDrag(float mx, float sliderWorldX);
    void UpdateDrag(float mx, float sliderWorldX);
    void EndDrag();

    bool IsDragging() const { return m_dragging; }

    void Render(Renderer& renderer) override;

private:
    float m_normalizedFromMouse(float mx, float sliderWorldX) const;

    float m_min = 0.1f;
    float m_max = 5.0f;
    float m_value = 1.0f;
    float m_normalized = 0.0f;

    bool m_hovered = false;
    bool m_dragging = false;

    std::function<void(float)> m_onChange;
};

} // namespace m2d