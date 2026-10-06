#pragma once

#include <vector>
#include <memory>
#include "ui/UIElement.h"

namespace m2d {

// Панель: прямоугольник с фоном, может содержать дочерние элементы.
// Дочерние элементы позиционируются ОТНОСИТЕЛЬНО панели.
class UIPanel : public UIElement {
public:
    UIPanel() = default;

    // ===== Цвет фона =====
    void SetBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) {
        m_bgR = r; m_bgG = g; m_bgB = b; m_bgA = a;
    }

    // ===== Граница =====
    void SetBorder(bool enabled, Uint8 r = 100, Uint8 g = 100, Uint8 b = 120) {
        m_hasBorder = enabled;
        m_borderR = r; m_borderG = g; m_borderB = b;
    }

    // ===== Дочерние элементы =====
    // Панель владеет дочерними элементами.
    // Возвращает указатель без владения — можно донастраивать.
    template<typename T, typename... Args>
    T* AddChild(Args&&... args) {
        auto child = std::make_unique<T>(std::forward<Args>(args)...);
        T* raw = child.get();
        m_children.push_back(std::move(child));
        return raw;
    }

    // ===== Жизненный цикл =====
    void Update(float dt) override;
    void Render(Renderer& renderer) override;

private:
    Uint8 m_bgR = 20, m_bgG = 20, m_bgB = 25, m_bgA = 240;
    bool  m_hasBorder = true;
    Uint8 m_borderR = 60, m_borderG = 60, m_borderB = 80;

    std::vector<std::unique_ptr<UIElement>> m_children;
};

} // namespace m2d