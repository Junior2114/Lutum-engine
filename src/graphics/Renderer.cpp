#include "graphics/Renderer.h"

#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>

namespace m2d {

// ===== Вспомогательная функция: Uint8 → SDL_FColor =====
// Обходит warning C4838 (narrowing conversion in braced-init-list).
static inline SDL_FColor ToColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_FColor c;
    c.r = static_cast<float>(r) / 255.0f;
    c.g = static_cast<float>(g) / 255.0f;
    c.b = static_cast<float>(b) / 255.0f;
    c.a = static_cast<float>(a) / 255.0f;
    return c;
}

void Renderer::Clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
    SDL_RenderClear(m_renderer);
}

// ===== Текстуры =====

void Renderer::DrawTexture(const Texture& texture, float x, float y) {
    if (!texture.IsValid()) return;
    SDL_FRect dst{ x, y,
                   (float)texture.GetWidth(),
                   (float)texture.GetHeight() };
    SDL_RenderTexture(m_renderer, texture.Get(), nullptr, &dst);
}

void Renderer::DrawTextureEx(const Texture& texture, float x, float y,
                             float scale) {
    if (!texture.IsValid()) return;
    SDL_FRect dst{ x, y,
                   (float)texture.GetWidth()  * scale,
                   (float)texture.GetHeight() * scale };
    SDL_RenderTexture(m_renderer, texture.Get(), nullptr, &dst);
}

void Renderer::DrawTextureRegion(const Texture& texture,
                                 const SDL_FRect& srcRect,
                                 const SDL_FRect& dstRect) {
    if (!texture.IsValid()) return;
    SDL_RenderTexture(m_renderer, texture.Get(), &srcRect, &dstRect);
}

// ===== Прямоугольник =====

void Renderer::DrawRect(const SDL_FRect& rect,
                        Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
    SDL_RenderFillRect(m_renderer, &rect);
}

// ===== Треугольник =====

void Renderer::DrawTriangleFilled(float x0, float y0,
                                  float x1, float y1,
                                  float x2, float y2,
                                  Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    const SDL_FColor color = ToColor(r, g, b, a);

    SDL_Vertex verts[3];
    verts[0].position = { x0, y0 };
    verts[0].color    = color;
    verts[0].tex_coord = { 0.0f, 0.0f };
    verts[1].position = { x1, y1 };
    verts[1].color    = color;
    verts[1].tex_coord = { 0.0f, 0.0f };
    verts[2].position = { x2, y2 };
    verts[2].color    = color;
    verts[2].tex_coord = { 0.0f, 0.0f };

    SDL_RenderGeometry(m_renderer, nullptr, verts, 3, nullptr, 0);
}

// ===== Эллипс =====

void Renderer::DrawEllipseFilled(float cx, float cy, float rx, float ry,
                                 Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                                 int segments) {
    if (rx <= 0.0f || ry <= 0.0f) return;

    // Адаптивное число сегментов, если не задано явно
    if (segments <= 0) {
        const float maxR = std::max(rx, ry);
        segments = std::clamp((int)(maxR * 2.5f), 64, 512);
    }
    if (segments < 3) segments = 3;

    const SDL_FColor color = ToColor(r, g, b, a);

    std::vector<SDL_Vertex> verts;
    verts.reserve(segments + 2);

    // Центр — вершина 0
    SDL_Vertex center;
    center.position = { cx, cy };
    center.color    = color;
    center.tex_coord = { 0.0f, 0.0f };
    verts.push_back(center);

    // Точки на окружности
    const float twoPi = 6.28318530718f;
    for (int i = 0; i <= segments; ++i) {
        const float angle = twoPi * (float)i / (float)segments;
        SDL_Vertex v;
        v.position = { cx + std::cos(angle) * rx,
                       cy + std::sin(angle) * ry };
        v.color    = color;
        v.tex_coord = { 0.0f, 0.0f };
        verts.push_back(v);
    }

    // Индексы — веер треугольников
    std::vector<int> indices;
    indices.reserve(segments * 3);
    for (int i = 1; i <= segments; ++i) {
        indices.push_back(0);
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    SDL_RenderGeometry(m_renderer, nullptr,
                       verts.data(), (int)verts.size(),
                       indices.data(), (int)indices.size());
}

// ===== Круг =====

void Renderer::DrawCircleFilled(float cx, float cy, float radius,
                                Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                                int segments) {
    DrawEllipseFilled(cx, cy, radius, radius, r, g, b, a, segments);
}

// ===== Текст =====

float Renderer::DrawString(const Font& font, const std::string& text,
                           float x, float y,
                           Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    if (!font.IsValid()) return 0.0f;
    if (text.empty())    return 0.0f;

    SDL_Color color{ r, g, b, a };

    SDL_Surface* surface = TTF_RenderText_Blended(font.Get(),
                                                   text.c_str(),
                                                   text.size(),
                                                   color);
    if (!surface) return 0.0f;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    if (!texture) {
        SDL_DestroySurface(surface);
        return 0.0f;
    }

    SDL_FRect dst{
        x,
        y,
        (float)surface->w,
        (float)surface->h
    };

    SDL_RenderTexture(m_renderer, texture, nullptr, &dst);

    float width = (float)surface->w;

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);

    return width;
}

void Renderer::Present() {
    SDL_RenderPresent(m_renderer);
}

} // namespace m2d