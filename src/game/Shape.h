#pragma once

#include <SDL3/SDL.h>
#include "graphics/Renderer.h"

namespace m2d {

class Shape {
public:
    enum class Type {
        Rectangle,
        Circle,
        Triangle
    };

    Shape(Type type, float x, float y, float w, float h,
          Uint8 r, Uint8 g, Uint8 b)
        : m_type(type), m_x(x), m_y(y), m_w(w), m_h(h)
        , m_r(r), m_g(g), m_b(b) {}

    void Render(Renderer& renderer) const;

    // ===== Попадание точки =====
    bool ContainsPoint(float px, float py) const {
        return px >= m_x && px <= m_x + m_w &&
               py >= m_y && py <= m_y + m_h;
    }

    // ===== Позиция =====
    void MoveTo(float x, float y) { m_x = x; m_y = y; }
    void MoveBy(float dx, float dy) { m_x += dx; m_y += dy; }

    // ===== Выделение =====
    void SetSelected(bool s) { m_selected = s; }
    bool IsSelected() const  { return m_selected; }

    // ===== Геттеры =====
    Type GetType() const { return m_type; }
    float GetX() const { return m_x; }
    float GetY() const { return m_y; }
    float GetW() const { return m_w; }
    float GetH() const { return m_h; }

private:
    Type  m_type;
    float m_x, m_y, m_w, m_h;
    Uint8 m_r, m_g, m_b;

    bool  m_selected = false;
};

} // namespace m2d