#include "ui/UILabel.h"

namespace m2d {

void UILabel::Render(Renderer& renderer) {
    if (!m_visible || !m_font || m_text.empty()) return;

    renderer.DrawString(*m_font, m_text, m_x, m_y, m_r, m_g, m_b, m_a);
}

} // namespace m2d