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
    const bool  mouseReleased = m_mouseWasDownLastFrame && !mouseDown;

    // Если тащим слайдер — обновляем его
    if (m_activeSlider) {
        if (mouseDown) {
            m_activeSlider->UpdateDrag(mx);
        } else {
            m_activeSlider->EndDrag();
            m_activeSlider = nullptr;
        }
    }

    for (auto& element : m_elements) {
        UpdateElement(element.get(), mx, my, mouseDown, mouseReleased);
    }

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

        if (mouseReleased && inside) {
            button->Click();
        }
        return;
    }

    // ===== Слайдер =====
    if (auto* slider = dynamic_cast<UISlider*>(element)) {
        const bool inside = slider->ContainsPoint(mx, my);
        slider->SetHovered(inside);

        // Начало drag: клик внутри слайдера + нет активного слайдера
        if (inside && mouseDown && !m_activeSlider && !slider->IsDragging()) {
            m_activeSlider = slider;
            slider->StartDrag(mx);
        }
        return;
    }

    // ===== Панель =====
    if (auto* panel = dynamic_cast<UIPanel*>(element)) {
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

    if (element->ContainsPoint(x, y)) {
        return true;
    }

    if (auto* panel = dynamic_cast<const UIPanel*>(element)) {
        const float px = panel->GetX();
        const float py = panel->GetY();

        for (const auto& child : panel->GetChildren()) {
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