#include "ui/UISeparator.h"

namespace m2d {

void UISeparator::Render(Renderer& renderer) {
    if (!m_visible) return;

    // Тонкая горизонтальная линия по центру элемента
    SDL_FRect line{ m_x, m_y + m_h * 0.5f, m_w, 1.0f };
    renderer.DrawRect(line, m_r, m_g, m_b, m_a);
}

} // namespace m2d