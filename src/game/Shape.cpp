#include "game/Shape.h"
#include <cmath>
#include <algorithm>

namespace m2d {

// ===== Вспомогательная функция: линия =====
static void DrawLine(Renderer& renderer,
                     float x0, float y0,
                     float x1, float y1,
                     Uint8 r, Uint8 g, Uint8 b,
                     float thickness = 2.0f) {
    const float dx = x1 - x0;
    const float dy = y1 - y0;

    const int steps = (int)std::max(std::abs(dx), std::abs(dy));
    if (steps <= 0) {
        renderer.DrawRect({ x0, y0, thickness, thickness }, r, g, b, 255);
        return;
    }

    for (int i = 0; i <= steps; ++i) {
        const float t = (float)i / (float)steps;
        const float x = x0 + dx * t;
        const float y = y0 + dy * t;
        renderer.DrawRect({ x - thickness * 0.5f, y - thickness * 0.5f,
                            thickness, thickness },
                          r, g, b, 255);
    }
}

static void DrawSelectionRectangle(Renderer& renderer,
                                   float x, float y, float w, float h,
                                   Uint8 r, Uint8 g, Uint8 b,
                                   float t = 2.0f) {
    renderer.DrawRect({ x - t, y - t, w + t * 2, t }, r, g, b, 255);
    renderer.DrawRect({ x - t, y + h, w + t * 2, t }, r, g, b, 255);
    renderer.DrawRect({ x - t, y - t, t, h + t * 2 }, r, g, b, 255);
    renderer.DrawRect({ x + w, y - t, t, h + t * 2 }, r, g, b, 255);
}

static void DrawSelectionCircle(Renderer& renderer,
                                float cx, float cy, float radius,
                                Uint8 r, Uint8 g, Uint8 b,
                                float t = 2.0f) {
    const int segments = std::max(32, (int)(radius * 1.5f));
    const float twoPi = 6.28318530718f;

    float prevX = cx + radius;
    float prevY = cy;

    for (int i = 1; i <= segments; ++i) {
        const float angle = twoPi * (float)i / (float)segments;
        const float px = cx + std::cos(angle) * radius;
        const float py = cy + std::sin(angle) * radius;
        DrawLine(renderer, prevX, prevY, px, py, r, g, b, t);
        prevX = px;
        prevY = py;
    }
}

static void DrawSelectionTriangle(Renderer& renderer,
                                  float x, float y, float w, float h,
                                  Uint8 r, Uint8 g, Uint8 b,
                                  float t = 2.0f) {
    const float topX    = x + w * 0.5f;
    const float topY    = y;
    const float leftX   = x;
    const float leftY   = y + h;
    const float rightX  = x + w;
    const float rightY  = y + h;

    DrawLine(renderer, topX,   topY,   leftX,  leftY,  r, g, b, t);
    DrawLine(renderer, leftX,  leftY,  rightX, rightY, r, g, b, t);
    DrawLine(renderer, rightX, rightY, topX,   topY,   r, g, b, t);
}

// ===== Основной Render =====

void Shape::Render(Renderer& renderer) const {
    const float w = GetW();
    const float h = GetH();

    switch (m_type) {
        case Type::Rectangle: {
            SDL_FRect rect{ m_x, m_y, w, h };
            renderer.DrawRect(rect, m_r, m_g, m_b, 255);
            break;
        }

        case Type::Circle: {
            const float cx = m_x + w * 0.5f;
            const float cy = m_y + h * 0.5f;
            const float radius = (w < h ? w : h) * 0.5f;

            const int steps = (int)(radius * 2.0f);
            if (steps <= 0) break;

            for (int i = -steps; i <= steps; ++i) {
                const float dy = (float)i / (float)steps * radius;
                const float r2 = radius * radius - dy * dy;
                if (r2 < 0.0f) continue;
                const float half = std::sqrt(r2);
                SDL_FRect row{ cx - half, cy + dy, half * 2.0f, 1.0f };
                renderer.DrawRect(row, m_r, m_g, m_b, 255);
            }
            break;
        }

        case Type::Triangle: {
            const int steps = (int)h;
            if (steps <= 0) break;

            for (int i = 0; i < steps; ++i) {
                const float t = (float)i / (float)steps;
                const float rowY = m_y + (float)i;
                const float halfWidth = (w * 0.5f) * t;
                const float centerX = m_x + w * 0.5f;
                SDL_FRect row{ centerX - halfWidth, rowY,
                               halfWidth * 2.0f, 1.0f };
                renderer.DrawRect(row, m_r, m_g, m_b, 255);
            }
            break;
        }
    }

    // ===== Обводка выделения =====
    if (m_selected) {
        constexpr Uint8 OR = 100, OG = 200, OB = 255;

        switch (m_type) {
            case Type::Rectangle:
                DrawSelectionRectangle(renderer, m_x, m_y, w, h, OR, OG, OB);
                break;

            case Type::Circle: {
                const float cx = m_x + w * 0.5f;
                const float cy = m_y + h * 0.5f;
                const float radius = (w < h ? w : h) * 0.5f;
                DrawSelectionCircle(renderer, cx, cy, radius + 1.0f,
                                    OR, OG, OB);
                break;
            }

            case Type::Triangle:
                DrawSelectionTriangle(renderer, m_x, m_y, w, h, OR, OG, OB);
                break;
        }
    }
}

} // namespace m2d