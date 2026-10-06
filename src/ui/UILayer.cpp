#include "ui/UILayer.h"
#include "core/Input.h"

namespace m2d {

void UILayer::Update(float dt, const Input& input) {
    if (!m_visible) return;

    // Получаем позицию мыши
    const float mx = input.GetMouseX();
    const float my = input.GetMouseY();

    // Обновляем элементы. Hover будет реализован в UIButton.
    for (auto& element : m_elements) {
        element->Update(dt);

        // Пока просто: если элемент — кнопка и мышь над ней, ставим hovered.
        // Это работает для плоской структуры; для вложенных — сделаем позже.
        // (Оставим задел: UIButton сам умеет SetHovered, но вызов здесь общий.)
    }
}

void UILayer::Render(Renderer& renderer) {
    if (!m_visible) return;

    for (auto& element : m_elements) {
        element->Render(renderer);
    }
}

bool UILayer::IsPointOverUI(float x, float y) const {
    if (!m_visible) return false;

    for (const auto& element : m_elements) {
        if (element->IsVisible() && element->ContainsPoint(x, y)) {
            return true;
        }
    }
    return false;
}

} // namespace m2d