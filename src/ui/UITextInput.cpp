#include "ui/UITextInput.h"

namespace m2d {

void UITextInput::SetFocused(bool f) {
    // При переходе false -> true очищаем, чтобы вводить с нуля
    if (f && !m_focused) {
        m_text.clear();
    }
    m_focused = f;
}

void UITextInput::InputCharacter(char c) {
    if (!m_focused) return;
    if (m_text.size() < 16) {
        m_text += c;
    }
}

void UITextInput::InputBackspace() {
    if (!m_focused) return;
    if (!m_text.empty()) {
        m_text.pop_back();
    }
}

void UITextInput::Submit() {
    if (m_onSubmit) m_onSubmit(m_text);
    m_focused = false;
    // НЕ очищаем — SetText позже обновит значение из shape
}

void UITextInput::Render(Renderer& renderer) {
    if (!m_visible) return;

    // Фон
    Uint8 bgR = 35, bgG = 35, bgB = 45;
    if (m_focused) {
        bgR = 50; bgG = 50; bgB = 65;
    } else if (m_hovered) {
        bgR = 45; bgG = 45; bgB = 55;
    }
    renderer.DrawRect(GetRect(), bgR, bgG, bgB, 255);

    // Рамка
    const Uint8 borderR = m_focused ? 100 : 60;
    const Uint8 borderG = m_focused ? 180 : 60;
    const Uint8 borderB = m_focused ? 255 : 80;

    renderer.DrawRect({ m_x, m_y, m_w, 1 }, borderR, borderG, borderB, 255);
    renderer.DrawRect({ m_x, m_y + m_h - 1, m_w, 1 }, borderR, borderG, borderB, 255);
    renderer.DrawRect({ m_x, m_y, 1, m_h }, borderR, borderG, borderB, 255);
    renderer.DrawRect({ m_x + m_w - 1, m_y, 1, m_h }, borderR, borderG, borderB, 255);

    // Текст
    if (m_font) {
        const std::string display = m_text + (m_focused ? "_" : "");
        renderer.DrawString(*m_font, display,
                            m_x + 8.0f,
                            m_y + (m_h - 20.0f) * 0.5f,
                            220, 220, 220);
    }
}

} // namespace m2d