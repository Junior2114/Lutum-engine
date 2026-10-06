#include "ui/UILayer.h"
#include "ui/UIPanel.h"
#include "core/Input.h"

namespace m2d {

void UILayer::Update(float dt, const Input& input) {
    if (!m_visible) return;

    const float mx = input.GetMouseX();
    const float my = input.GetMouseY();
    const bool mouseDown = input.IsMouseButtonDown(SDL_BUTTON_LEFT);

    for (auto& element : m_elements) {
        UpdateElement(element.get(), input, mx, my, mouseDown);
    }
}

void UILayer::UpdateElement(UIElement* element, const Input& input,
                            float mx, float my, bool mouseDown) {
    if (!element || !element->IsVisible()) return;

    // ===== Если это кнопка — обрабатываем =====
    if (auto* button = dynamic_cast<UIButton*>(element)) {
        const bool inside = button->ContainsPoint(mx, my);

        button->SetHovered(inside);

        // Клик — только когда кнопка под мышью И мышь зажата
        button->SetPressed(inside && mouseDown);

        // Отпускание мыши внутри кнопки = клик
        static bool wasDown = false;
        if (wasDown && !mouseDown && inside) {
            button->Click();
        }
        wasDown = mouseDown;

        return;   // кнопка — лист, дальше не идём
    }

    // ===== Если это панель — обходим детей =====
    if (auto* panel = dynamic_cast<UIPanel*>(element)) {
        // Панель хранит детей внутри. Нужен доступ к ним.
        // Простой способ — панель сама должна уметь обрабатывать клики,
        // но у нас UILayer владеет элементами, а не панель.
        //
        // Пока работаем через прямое обращение к детям панели.
        // Для этого в UIPanel есть метод GetChildren().
        for (auto& child : panel->GetChildren()) {
            UpdateElement(child.get(), input, mx, my, mouseDown);
        }
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