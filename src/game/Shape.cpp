#include "game/Shape.h"
#include <cmath>
#include <algorithm>

namespace m2d {

static void DrawLine(Renderer& renderer,
                     float x0, float y0, float x1, float y1,
                     Uint8 r, Uint8 g, Uint8 b, float t = 2.0f) {
    const float dx = x1 - x0;
    const float dy = y1 - y0;
    const int steps = (int)std::max(std::abs(dx), std::abs(dy));
    if (steps <= 0) {
        renderer.DrawRect({ x0, y0, t, t }, r, g, b, 255);
        return;
    }
    for (int i = 0; i <= steps; ++i) {
        const float a = (float)i / (float)steps;
        renderer.DrawRect({ x0 + dx * a - t * 0.5f,
                            y0 + dy * a - t * 0.5f, t, t },
                          r, g, b, 255);
    }
}

void Shape::Render(Renderer& renderer) const {
    const float x = GetX();
    const float y = GetY();
    const float w = GetW();
    const float h = GetH();

    switch (m_type) {
        case Type::Rectangle: {
            renderer.DrawRect({ x, y, w, h }, m_r, m_g, m_b, 255);
            break;
        }

        case Type::Circle: {
            // Используем DrawEllipseFilled — он даёт гладкий круг через GPU.
            // Круг — частный случай эллипса с rx == ry.
            const float cx = x + w * 0.5f;
            const float cy = y + h * 0.5f;
            const float rx = w * 0.5f;
            const float ry = h * 0.5f;

            // Чем больше круг, тем больше сегментов.
            // 64 — достаточно для любого разумного размера.
            renderer.DrawEllipseFilled(cx, cy, rx, ry,
                                       m_r, m_g, m_b, 255,
                                       64);
            break;
        }

        case Type::Triangle: {
            // Один вызов DrawTriangleFilled — три вершины.
            const float x0 = x + w * 0.5f;
            const float y0 = y;
            const float x1 = x;
            const float y1 = y + h;
            const float x2 = x + w;
            const float y2 = y + h;

            renderer.DrawTriangleFilled(x0, y0, x1, y1, x2, y2,
                                        m_r, m_g, m_b, 255);
            break;
        }
    }

    // ===== Обводка выделения =====
    if (m_selected) {
        constexpr Uint8 OR = 100, OG = 200, OB = 255;
        constexpr float T = 2.5f;

        switch (m_type) {
            case Type::Rectangle:
                renderer.DrawRect({ x - T, y - T, w + T * 2, T }, OR, OG, OB, 255);
                renderer.DrawRect({ x - T, y + h, w + T * 2, T }, OR, OG, OB, 255);
                renderer.DrawRect({ x - T, y - T, T, h + T * 2 }, OR, OG, OB, 255);
                renderer.DrawRect({ x + w, y - T, T, h + T * 2 }, OR, OG, OB, 255);
                break;

            case Type::Circle: {
				const float cx = x + w * 0.5f;
				const float cy = y + h * 0.5f;
				const float rx = w * 0.5f;
				const float ry = h * 0.5f;

				// segments = 0 → авто (адаптивно к радиусу)
				renderer.DrawEllipseFilled(cx, cy, rx, ry, m_r, m_g, m_b, 255);
				break;
			}

            case Type::Triangle:
                DrawLine(renderer, x + w * 0.5f, y, x, y + h, OR, OG, OB, T);
                DrawLine(renderer, x, y + h, x + w, y + h, OR, OG, OB, T);
                DrawLine(renderer, x + w, y + h, x + w * 0.5f, y, OR, OG, OB, T);
                break;
        }
    }
}

} // namespace m2d