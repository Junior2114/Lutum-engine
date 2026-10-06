#include "ui/UILayer.h"
#include "ui/UIPanel.h"
#include "core/Input.h"

namespace m2d {

void UILayer::Update(float dt, const Input& input) {
    (void)dt;
    if (!m_visible) return;

    const float mx = input.GetMouseX();
    const float my = input.GetMouseY();
    const bool mouseDown = input.IsMouseButtonDown(SDL_BUTTON_LEFT);
    const bool mouseReleased = m_mouseWasDownLastFrame && !mouseDown;

    // ===== Активный слайдер =====
    if (m_activeSlider) {
        if (mouseDown) {
            m_activeSlider->UpdateDrag(mx, m_activeSliderWorldX);
        } else {
            m_activeSlider->EndDrag();
            m_activeSlider = nullptr;
        }
    }

    // ===== Фокусное текстовое поле =====
    if (m_focusedInput) {
        for (char c : input.GetTextInput()) {
            m_focusedInput->InputCharacter(c);
        }
        if (input.WasBackspacePressed()) {
            m_focusedInput->InputBackspace();
        }
        if (input.WasEnterPressed()) {
            m_focusedInput->Submit();
            m_focusedInput = nullptr;
        }
    }

    for (auto& element : m_elements) {
        UpdateElement(element.get(), mx, my, mouseDown, mouseReleased,
                      input, 0.0f, 0.0f);
    }

    m_mouseWasDownLastFrame = mouseDown;
}

void UILayer::UpdateElement(UIElement* element,
                            float mx, float my,
                            bool mouseDown, bool mouseReleased,
                            const Input& input,
                            float parentOffsetX, float parentOffsetY) {
    if (!element || !element->IsVisible()) return;

    const float worldX = element->GetX() + parentOffsetX;
    const float worldY = element->GetY() + parentOffsetY;

    // ===== Кнопка =====
    if (auto* button = dynamic_cast<UIButton*>(element)) {
        const bool inside = mx >= worldX && mx <= worldX + button->GetW() &&
                            my >= worldY && my <= worldY + button->GetH();
        button->SetHovered(inside);
        button->SetPressed(inside && mouseDown);
        if (mouseReleased && inside) {
            if (m_focusedInput) { m_focusedInput->SetFocused(false); m_focusedInput = nullptr; }
            button->Click();
        }
        return;
    }

    // ===== Слайдер =====
    if (auto* slider = dynamic_cast<UISlider*>(element)) {
        if (!m_activeSlider && !m_focusedInput) {
            const float clickTop = worldY - 6.0f;
            const float clickBot = worldY + slider->GetH() + 6.0f;
            const bool inside = mx >= worldX && mx <= worldX + slider->GetW() &&
                                my >= clickTop && my <= clickBot;
            slider->SetHovered(inside);
            if (inside && mouseDown) {
                m_activeSlider = slider;
                m_activeSliderWorldX = worldX;
                slider->StartDrag(mx, worldX);
            }
        }
        return;
    }

    // ===== Текстовое поле =====
    if (auto* textInput = dynamic_cast<UITextInput*>(element)) {
        const bool inside = mx >= worldX && mx <= worldX + textInput->GetW() &&
                            my >= worldY && my <= worldY + textInput->GetH();
        textInput->SetHovered(inside);

        if (mouseReleased && inside) {
            // Снять фокус со старого
            if (m_focusedInput && m_focusedInput != textInput) {
                m_focusedInput->SetFocused(false);
            }
            m_focusedInput = textInput;
            textInput->SetFocused(true);
        } else if (mouseReleased && !inside && m_focusedInput == textInput) {
            // Клик вне поля — снять фокус
            textInput->Submit();
            m_focusedInput = nullptr;
        }
        return;
    }

    // ===== Панель =====
    if (auto* panel = dynamic_cast<UIPanel*>(element)) {
        for (auto& child : panel->GetChildrenMut()) {
            UpdateElement(child.get(), mx, my, mouseDown, mouseReleased,
                          input, worldX, worldY);
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
        if (IsPointOverElement(element.get(), x, y)) return true;
    }
    return false;
}

bool UILayer::IsPointOverElement(const UIElement* element,
                                 float x, float y) const {
    if (!element || !element->IsVisible()) return false;
    if (element->ContainsPoint(x, y)) return true;

    if (auto* panel = dynamic_cast<const UIPanel*>(element)) {
        const float px = panel->GetX();
        const float py = panel->GetY();
        for (const auto& child : panel->GetChildren()) {
            SDL_FRect r = child->GetRect();
            r.x += px; r.y += py;
            if (x >= r.x && x <= r.x + r.w && y >= r.y && y <= r.y + r.h) return true;
        }
    }
    return false;
}

} // namespace m2d