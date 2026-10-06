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

    // ===== Фон панели =====
    SDL_FRect bg = GetRect();
    renderer.DrawRect(bg, m_bgR, m_bgG, m_bgB, m_bgA);

    // ===== Граница справа (вертикальная полоса) =====
    if (m_hasBorder) {
        SDL_FRect border{ m_x + m_w - 1.0f, m_y, 1.0f, m_h };
        renderer.DrawRect(border, m_borderR, m_borderG, m_borderB, 255);
    }

    // ===== Дочерние элементы =====
    // Дети рисуются относительно панели: их (x, y) — это смещение внутри панели.
    // Мы временно смещаем их абсолютные координаты.
    for (auto& child : m_children) {
        // Сохраняем исходную позицию
        const float savedX = child->GetX();
        const float savedY = child->GetY();

        // Смещаем на позицию панели
        child->SetPosition(m_x + savedX, m_y + savedY);
        child->Render(renderer);

        // Возвращаем назад
        child->SetPosition(savedX, savedY);
    }
}

} // namespace m2d