#pragma once

#include <SDL3/SDL.h>
#include <string>
#include "graphics/Renderer.h"

namespace m2d {

class Shape {
public:
    enum class Type {
        Rectangle,
        Circle,
        Triangle
    };

    // Базовый размер одинаков для всех фигур.
    static constexpr float BASE_SIZE = 80.0f;

    Shape(Type type, float x, float y,
          Uint8 r, Uint8 g, Uint8 b)
        : m_type(type), m_x(x), m_y(y)
        , m_r(r), m_g(g), m_b(b) {}

    void Render(Renderer& renderer) const;

    // ===== Попадание =====
    bool ContainsPoint(float px, float py) const {
        return px >= m_x && px <= m_x + GetW() &&
               py >= m_y && py <= m_y + GetH();
    }

    // ===== Позиция =====
    void MoveTo(float x, float y) { m_x = x; m_y = y; }
    void MoveBy(float dx, float dy) { m_x += dx; m_y += dy; }

    // ===== Размер =====
    // Реальный размер = BASE_SIZE * scale.
    // Scale меняется через инспектор.
    float GetW() const { return BASE_SIZE * m_scale; }
    float GetH() const { return BASE_SIZE * m_scale; }
    float GetBaseSize() const { return BASE_SIZE; }

    void SetScale(float s) {
        if (s < 0.1f)  s = 0.1f;
        if (s > 10.0f) s = 10.0f;
        m_scale = s;
    }
    float GetScale() const { return m_scale; }

    // ===== Цвет =====
    void SetColor(Uint8 r, Uint8 g, Uint8 b) {
        m_r = r; m_g = g; m_b = b;
    }
    Uint8 GetR() const { return m_r; }
    Uint8 GetG() const { return m_g; }
    Uint8 GetB() const { return m_b; }

    // ===== Выделение =====
    void SetSelected(bool s) { m_selected = s; }
    bool IsSelected() const  { return m_selected; }

    // ===== Геттеры =====
    Type GetType() const { return m_type; }
    float GetX() const { return m_x; }
    float GetY() const { return m_y; }

    const char* GetTypeName() const {
        switch (m_type) {
            case Type::Rectangle: return "Rectangle";
            case Type::Circle:    return "Circle";
            case Type::Triangle:  return "Triangle";
        }
        return "Unknown";
    }

private:
    Type  m_type;
    float m_x = 0.0f;
    float m_y = 0.0f;
    float m_scale = 1.0f;   // множитель базового размера
    Uint8 m_r, m_g, m_b;

    bool  m_selected = false;
};

} // namespace m2d