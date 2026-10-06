#pragma once

#include <functional>
#include "ui/UIElement.h"

namespace m2d {

// Горизонтальный слайдер. Значение от 0 до 1, маппится в [min, max].
class UISlider : public UIElement {
public:
    UISlider() = default;

    // ===== Настройка =====
    void SetRange(float minV, float maxV) {
        m_min = minV; m_max = maxV;
    }
    void SetValue(float value) {
        if (value < m_min) value = m_min;
        if (value > m_max) value = m_max;
        m_value = value;
        m_normalized = (m_max > m_min)
                       ? (m_value - m_min) / (m_max - m_min)
                       : 0.0f;
    }
    float GetValue() const { return m_value; }

    // ===== Обработчик изменения =====
    void SetOnChange(std::function<void(float)> callback) {
        m_onChange = std::move(callback);
    }

    // ===== Состояние (управляется UILayer) =====
    void SetHovered(bool h) { m_hovered = h; }
    bool IsHovered() const  { return m_hovered; }

    // Начать drag ползунка. Вызывается UILayer при клике внутри слайдера.
    void StartDrag(float mouseX);
    void UpdateDrag(float mouseX);
    void EndDrag();

    bool IsDragging() const { return m_dragging; }

    // ===== Жизненный цикл =====
    void Render(Renderer& renderer) override;

private:
    float m_normalizedFromMouse(float mouseX) const;

    float m_min = 0.1f;
    float m_max = 5.0f;
    float m_value = 1.0f;
    float m_normalized = 0.0f;   // [0..1]

    bool m_hovered = false;
    bool m_dragging = false;

    std::function<void(float)> m_onChange;
};

} // namespace m2d