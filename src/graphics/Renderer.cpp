#include "graphics/Renderer.h"

#include <SDL3_ttf/SDL_ttf.h>
#include <string>

namespace m2d {

void Renderer::Clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
    SDL_RenderClear(m_renderer);
}

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

void Renderer::DrawRect(const SDL_FRect& rect,
                        Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
    SDL_RenderFillRect(m_renderer, &rect);
}

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