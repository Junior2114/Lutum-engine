#pragma once

#include <SDL3/SDL.h>
#include <string>
#include "graphics/Texture.h"
#include "graphics/Font.h"

namespace m2d {

class Renderer {
public:
    Renderer() = default;
    explicit Renderer(SDL_Renderer* renderer) : m_renderer(renderer) {}

    void Clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

    void DrawTexture(const Texture& texture, float x, float y);
    void DrawTextureEx(const Texture& texture, float x, float y, float scale);
    void DrawTextureRegion(const Texture& texture,
                           const SDL_FRect& srcRect,
                           const SDL_FRect& dstRect);

    void DrawRect(const SDL_FRect& rect,
                  Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

    void DrawTriangleFilled(float x0, float y0,
                            float x1, float y1,
                            float x2, float y2,
                            Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

    // segments = 0 → авто (зависит от радиуса)
    void DrawCircleFilled(float cx, float cy, float radius,
                          Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255,
                          int segments = 0);

    void DrawEllipseFilled(float cx, float cy, float rx, float ry,
                           Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255,
                           int segments = 0);

    float DrawString(const Font& font, const std::string& text,
                     float x, float y,
                     Uint8 r = 255, Uint8 g = 255, Uint8 b = 255, Uint8 a = 255);

    void Present();
    SDL_Renderer* Get() const { return m_renderer; }

private:
    SDL_Renderer* m_renderer = nullptr;
};

} // namespace m2d