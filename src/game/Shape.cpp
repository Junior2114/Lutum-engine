#include "game/Shape.h"

namespace m2d {

void Shape::Render(Renderer& renderer) const {
    // ===== Тело фигуры =====
    switch (m_type) {
        case Type::Rectangle: {
            SDL_FRect rect{ m_x, m_y, m_w, m_h };
            renderer.DrawRect(rect, m_r, m_g, m_b, 255);
            break;
        }

        case Type::Circle: {
            const float cx = m_x + m_w * 0.5f;
            const float cy = m_y + m_h * 0.5f;
            const float radius = (m_w < m_h ? m_w : m_h) * 0.5f;

            const int steps = (int)radius * 2;
            for (int i = -steps; i <= steps; ++i) {
                const float dy = (float)i / steps * radius;
                const float half = SDL_sqrtf(radius * radius - dy * dy);
                SDL_FRect row{ cx - half, cy + dy, half * 2.0f, 1.0f };
                renderer.DrawRect(row, m_r, m_g, m_b, 255);
            }
            break;
        }

        case Type::Triangle: {
            // Пока как прямоугольник — заменим позже через SDL_RenderGeometry
            SDL_FRect rect{ m_x, m_y, m_w, m_h };
            renderer.DrawRect(rect, m_r, m_g, m_b, 255);
            break;
        }
    }

    // ===== Обводка выделения =====
    if (m_selected) {
        constexpr float THICK = 2.0f;
        const Uint8 OR = 100, OG = 200, OB = 255;   // голубой

        // Верхняя линия
        renderer.DrawRect({ m_x - THICK, m_y - THICK, m_w + THICK * 2, THICK },
                          OR, OG, OB, 255);
        // Нижняя
        renderer.DrawRect({ m_x - THICK, m_y + m_h, m_w + THICK * 2, THICK },
                          OR, OG, OB, 255);
        // Левая
        renderer.DrawRect({ m_x - THICK, m_y - THICK, THICK, m_h + THICK * 2 },
                          OR, OG, OB, 255);
        // Правая
        renderer.DrawRect({ m_x + m_w, m_y - THICK, THICK, m_h + THICK * 2 },
                          OR, OG, OB, 255);
    }
}

} // namespace m2d