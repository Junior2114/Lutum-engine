#pragma once

#include <string>
#include <functional>
#include "ui/UIElement.h"
#include "graphics/Font.h"

namespace m2d {

class UITextInput : public UIElement {
public:
    UITextInput() = default;

    void SetFont(Font* font) { m_font = font; }

    // SetText НЕ работает, пока поле в фокусе — защита от перезаписи
    void SetText(const std::string& text) {
        if (m_focused) return;
        m_text = text;
    }
    const std::string& GetText() const { return m_text; }

    void SetOnSubmit(std::function<void(const std::string&)> cb) {
        m_onSubmit = std::move(cb);
    }

    void SetFocused(bool f);
    bool IsFocused() const { return m_focused; }
    void SetHovered(bool h) { m_hovered = h; }

    void InputCharacter(char c);
    void InputBackspace();
    void Submit();

    void Render(Renderer& renderer) override;

private:
    Font* m_font = nullptr;
    std::string m_text;
    bool m_focused = false;
    bool m_hovered = false;

    std::function<void(const std::string&)> m_onSubmit;
};

} // namespace m2d