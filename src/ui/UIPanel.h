#pragma once

#include <vector>
#include <memory>
#include "ui/UIElement.h"

namespace m2d {

class UIPanel : public UIElement {
public:
    UIPanel() = default;

    void SetBackgroundColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255) {
        m_bgR = r; m_bgG = g; m_bgB = b; m_bgA = a;
    }

    void SetBorder(bool enabled, Uint8 r = 100, Uint8 g = 100, Uint8 b = 120) {
        m_hasBorder = enabled;
        m_borderR = r; m_borderG = g; m_borderB = b;
    }

    template<typename T, typename... Args>
    T* AddChild(Args&&... args) {
        auto child = std::make_unique<T>(std::forward<Args>(args)...);
        T* raw = child.get();
        m_children.push_back(std::move(child));
        return raw;
    }

    // ===== Доступ к детям (для UILayer) =====
    const std::vector<std::unique_ptr<UIElement>>& GetChildren() const {
        return m_children;
    }

    // Нужно для UpdateElement — вернуть мутабельную ссылку
    std::vector<std::unique_ptr<UIElement>>& GetChildrenMut() {
        return m_children;
    }

    void Update(float dt) override;
    void Render(Renderer& renderer) override;

private:
    Uint8 m_bgR = 20, m_bgG = 20, m_bgB = 25, m_bgA = 240;
    bool  m_hasBorder = true;
    Uint8 m_borderR = 60, m_borderG = 60, m_borderB = 80;

    std::vector<std::unique_ptr<UIElement>> m_children;
};

} // namespace m2d