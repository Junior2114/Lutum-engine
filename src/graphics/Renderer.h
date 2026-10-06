#pragma once

#include <SDL3/SDL.h>
#include "graphics/Texture.h"

namespace m2d {

class Renderer {
public:
    Renderer() = default;
    explicit Renderer(SDL_Renderer* renderer) : m_renderer(renderer) {}

    void Clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

    void DrawTexture(const Texture& texture, float x, float y);

    void DrawTextureEx(const Texture& texture, float x, float y,
                       float scale);

    void DrawTextureRegion(const Texture& texture,
                           const SDL_FRect& srcRect,
                           const SDL_FRect& dstRect);

    void DrawRect(const SDL_FRect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);

    void Present();

    SDL_Renderer* Get() const { return m_renderer; }

private:
    SDL_Renderer* m_renderer = nullptr;
};

} // namespace m2d