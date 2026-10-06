#include "ui/UIPanel.h"

namespace m2d {

void UIPanel::Update(float dt) {
    if (!m_visible) return;
    for (auto& child : m_children) {
        child->Update(dt);
    }
}

void UIPanel::Render(Renderer& renderer) {
    if (!m_visible) return;

    // ===== Фон =====
    SDL_FRect bg = GetRect();
    renderer.DrawRect(bg, m_bgR, m_bgG, m_bgB, m_bgA);

    // ===== Граница справа =====
    if (m_hasBorder) {
        SDL_FRect border{ m_x + m_w - 1.0f, m_y, 1.0f, m_h };
        renderer.DrawRect(border, m_borderR, m_borderG, m_borderB, 255);
    }

    // ===== Дети — во временных мировых координатах =====
    for (auto& child : m_children) {
        const float savedX = child->GetX();
        const float savedY = child->GetY();

        child->SetPosition(savedX + m_x, savedY + m_y);
        child->Render(renderer);
        child->SetPosition(savedX, savedY);
    }
}

} // namespace m2d