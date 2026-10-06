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
            const float cx = x + w * 0.5f;
            const float cy = y + h * 0.5f;
            const float rx = w * 0.5f;
            const float ry = h * 0.5f;
            const int steps = (int)(ry * 2.0f);
            if (steps <= 0) break;
            for (int i = -steps; i <= steps; ++i) {
                const float dy = (float)i / (float)steps * ry;
                const float r2 = 1.0f - (dy * dy) / (ry * ry);
                if (r2 < 0.0f) continue;
                const float half = rx * std::sqrt(r2);
                renderer.DrawRect({ cx - half, cy + dy, half * 2.0f, 1.0f },
                                  m_r, m_g, m_b, 255);
            }
            break;
        }
        case Type::Triangle: {
            const int steps = (int)h;
            if (steps <= 0) break;
            for (int i = 0; i < steps; ++i) {
                const float t = (float)i / (float)steps;
                const float halfW = (w * 0.5f) * t;
                const float cx = x + w * 0.5f;
                renderer.DrawRect({ cx - halfW, y + i, halfW * 2.0f, 1.0f },
                                  m_r, m_g, m_b, 255);
            }
            break;
        }
    }

    // ===== Обводка выделения =====
    if (m_selected) {
        constexpr Uint8 OR = 100, OG = 200, OB = 255;
        constexpr float T = 2.0f;

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
                const float rx = w * 0.5f + 1.0f;
                const float ry = h * 0.5f + 1.0f;
                const int segs = std::max(32, (int)(std::max(rx, ry) * 1.5f));
                const float twoPi = 6.28318530718f;
                float px = cx + rx, py = cy;
                for (int i = 1; i <= segs; ++i) {
                    const float a = twoPi * (float)i / (float)segs;
                    const float nx = cx + std::cos(a) * rx;
                    const float ny = cy + std::sin(a) * ry;
                    DrawLine(renderer, px, py, nx, ny, OR, OG, OB, T);
                    px = nx; py = ny;
                }
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