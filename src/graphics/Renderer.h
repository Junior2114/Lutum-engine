#pragma once

#include <SDL3/SDL.h>
#include "graphics/Texture.h"

// Обёртка над SDL_Renderer*.
// НЕ владеет рендерером — владелец Application.
// Просто даёт удобный API для отрисовки.
class Renderer {
public:
    Renderer() = default;
    explicit Renderer(SDL_Renderer* renderer) : m_renderer(renderer) {}

    // Очистить экран цветом
    void Clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

    // Нарисовать текстуру в точке (x, y) в её исходном размере
    void DrawTexture(const Texture& texture, float x, float y);

    // Нарисовать текстуру с масштабом
    void DrawTextureEx(const Texture& texture, float x, float y,
                       float scale);

    // Нарисовать часть текстуры (для спрайт-листов / анимаций)
    void DrawTextureRegion(const Texture& texture,
                           const SDL_FRect& srcRect,
                           const SDL_FRect& dstRect);

    // Нарисовать залитый прямоугольник (для отладки)
    void DrawRect(const SDL_FRect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

    // Показать кадр
    void Present();

    SDL_Renderer* Get() const { return m_renderer; }

private:
    SDL_Renderer* m_renderer = nullptr;
};