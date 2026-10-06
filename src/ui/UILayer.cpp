#include "ui/UILayer.h"
#include "ui/UIPanel.h"
#include "core/Input.h"

namespace m2d {

void UILayer::Update(float dt, const Input& input) {
    (void)dt;
    if (!m_visible) return;

    const float mx = input.GetMouseX();
    const float my = input.GetMouseY();
    const bool  mouseDown = input.IsMouseButtonDown(SDL_BUTTON_LEFT);

    // Клик = мышь была нажата в ПРОШЛОМ кадре и отпущена СЕЙЧАС.
    // Это вычисляется ОДИН РАЗ за кадр — до обхода элементов.
    const bool mouseReleased = m_mouseWasDownLastFrame && !mouseDown;

    for (auto& element : m_elements) {
        UpdateElement(element.get(), mx, my, mouseDown, mouseReleased);
    }

    // Обновляем состояние ТОЛЬКО ОДИН РАЗ, после всех элементов.
    m_mouseWasDownLastFrame = mouseDown;
}

void UILayer::UpdateElement(UIElement* element,
                            float mx, float my,
                            bool mouseDown, bool mouseReleased) {
    if (!element || !element->IsVisible()) return;

    // ===== Кнопка =====
    if (auto* button = dynamic_cast<UIButton*>(element)) {
        const bool inside = button->ContainsPoint(mx, my);

        button->SetHovered(inside);
        button->SetPressed(inside && mouseDown);

        // Клик: мышь отпущена внутри кнопки
        if (mouseReleased && inside) {
            button->Click();
        }
        return;
    }

    // ===== Панель =====
    if (auto* panel = dynamic_cast<UIPanel*>(element)) {
        // Дети позиционируются ОТНОСИТЕЛЬНО панели.
        // Для hit-теста прибавляем позицию панели к их координатам.
        const float px = panel->GetX();
        const float py = panel->GetY();

        for (auto& child : panel->GetChildrenMut()) {
            const float savedX = child->GetX();
            const float savedY = child->GetY();

            child->SetPosition(savedX + px, savedY + py);
            UpdateElement(child.get(), mx, my, mouseDown, mouseReleased);
            child->SetPosition(savedX, savedY);
        }
        return;
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
        if (IsPointOverElement(element.get(), x, y)) {
            return true;
        }
    }
    return false;
}

bool UILayer::IsPointOverElement(const UIElement* element,
                                 float x, float y) const {
    if (!element || !element->IsVisible()) return false;

    // Проверяем попадание в сам элемент
    if (element->ContainsPoint(x, y)) {
        return true;
    }

    // Если это панель — рекурсивно проверяем детей
    if (auto* panel = dynamic_cast<const UIPanel*>(element)) {
        const float px = panel->GetX();
        const float py = panel->GetY();

        for (const auto& child : panel->GetChildren()) {
            // Временно смещаем — но так как метод const, используем
            // альтернативный способ: проверяем, попадает ли точка
            // в child с учётом смещения панели.
            SDL_FRect r = child->GetRect();
            r.x += px;
            r.y += py;

            if (x >= r.x && x <= r.x + r.w &&
                y >= r.y && y <= r.y + r.h) {
                return true;
            }
        }
    }

    return false;
}

} // namespace m2d