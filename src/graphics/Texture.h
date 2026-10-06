#pragma once

#include <SDL3/SDL.h>
#include <string>

namespace m2d {

class Texture {
public:
    Texture() = default;
    ~Texture();

    bool LoadFromFile(SDL_Renderer* renderer, const std::string& path);

    bool Create(SDL_Renderer* renderer, int width, int height,
                SDL_PixelFormat format = SDL_PIXELFORMAT_RGBA8888);

    void Destroy();

    SDL_Texture* Get() const { return m_texture; }

    int GetWidth()  const { return m_width; }
    int GetHeight() const { return m_height; }

    bool IsValid() const { return m_texture != nullptr; }

    Texture(const Texture&)            = delete;
    Texture& operator=(const Texture&) = delete;

    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

private:
    SDL_Texture* m_texture = nullptr;
    int m_width  = 0;
    int m_height = 0;
};

} // namespace m2d