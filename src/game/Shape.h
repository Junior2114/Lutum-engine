#pragma once

#include <SDL3/SDL.h>
#include "graphics/Renderer.h"

namespace m2d {

class Shape {
public:
    enum class Type { Rectangle, Circle, Triangle };

    static constexpr float BASE_SIZE = 80.0f;

    Shape(Type type, float centerX, float centerY,
          Uint8 r, Uint8 g, Uint8 b)
        : m_type(type), m_centerX(centerX), m_centerY(centerY)
        , m_r(r), m_g(g), m_b(b) {}

    void Render(Renderer& renderer) const;

    // ===== Размер (из центра, независимо по X и Y) =====
    float GetW() const { return BASE_SIZE * m_scaleX; }
    float GetH() const { return BASE_SIZE * m_scaleY; }

    float GetX() const { return m_centerX - GetW() * 0.5f; }
    float GetY() const { return m_centerY - GetH() * 0.5f; }
    float GetCenterX() const { return m_centerX; }
    float GetCenterY() const { return m_centerY; }

    void SetCenter(float cx, float cy) { m_centerX = cx; m_centerY = cy; }
    void MoveBy(float dx, float dy) { m_centerX += dx; m_centerY += dy; }

    bool ContainsPoint(float px, float py) const {
        return px >= GetX() && px <= GetX() + GetW() &&
               py >= GetY() && py <= GetY() + GetH();
    }

    // ===== Scale X / Y =====
    void SetScaleX(float s) {
        if (s < 0.1f)  s = 0.1f;
        if (s > 10.0f) s = 10.0f;
        m_scaleX = s;
    }
    void SetScaleY(float s) {
        if (s < 0.1f)  s = 0.1f;
        if (s > 10.0f) s = 10.0f;
        m_scaleY = s;
    }
    float GetScaleX() const { return m_scaleX; }
    float GetScaleY() const { return m_scaleY; }

    // ===== Цвет =====
    void SetColor(Uint8 r, Uint8 g, Uint8 b) { m_r = r; m_g = g; m_b = b; }
    Uint8 GetR() const { return m_r; }
    Uint8 GetG() const { return m_g; }
    Uint8 GetB() const { return m_b; }

    // ===== Выделение =====
    void SetSelected(bool s) { m_selected = s; }
    bool IsSelected() const  { return m_selected; }

    // ===== Тип =====
    Type GetType() const { return m_type; }
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
    float m_centerX = 0.0f;
    float m_centerY = 0.0f;
    float m_scaleX  = 1.0f;
    float m_scaleY  = 1.0f;
    Uint8 m_r, m_g, m_b;
    bool  m_selected = false;
};

} // namespace m2d