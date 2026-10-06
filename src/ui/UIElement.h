#pragma once

#include <SDL3/SDL.h>
#include "graphics/Renderer.h"

namespace m2d {

// Базовый элемент UI: прямоугольник с позицией и видимостью.
// Наследники: UIPanel, UIButton, UILabel.
class UIElement {
public:
    virtual ~UIElement() = default;

    // ===== Геометрия =====
    void SetPosition(float x, float y) { m_x = x; m_y = y; }
    void SetSize(float w, float h)     { m_w = w; m_h = h; }
    void SetRect(const SDL_FRect& r)   { m_x = r.x; m_y = r.y; m_w = r.w; m_h = r.h; }

    float GetX() const { return m_x; }
    float GetY() const { return m_y; }
    float GetW() const { return m_w; }
    float GetH() const { return m_h; }

    SDL_FRect GetRect() const { return { m_x, m_y, m_w, m_h }; }

    // ===== Видимость =====
    void SetVisible(bool v) { m_visible = v; }
    bool IsVisible() const  { return m_visible; }

    // ===== Жизненный цикл =====
    // Вызывается каждый кадр. Для кнопок — проверка клика, hover и т.д.
    virtual void Update(float dt) { (void)dt; }

    // Отрисовка. Каждый элемент знает, как себя рисовать.
    virtual void Render(Renderer& renderer) = 0;

    // Попадает ли точка в прямоугольник элемента (для кликов).
    virtual bool ContainsPoint(float px, float py) const {
        return px >= m_x && px <= m_x + m_w &&
               py >= m_y && py <= m_y + m_h;
    }

protected:
    float m_x = 0.0f;
    float m_y = 0.0f;
    float m_w = 0.0f;
    float m_h = 0.0f;
    bool  m_visible = true;
};

} // namespace m2d