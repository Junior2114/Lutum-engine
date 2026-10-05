#include "graphics/Texture.h"

#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <utility>

Texture::~Texture() {
    Destroy();
}

bool Texture::LoadFromFile(SDL_Renderer* renderer, const std::string& path) {
    Destroy();  // на случай повторной загрузки в этот же объект

    SDL_Texture* loaded = IMG_LoadTexture(renderer, path.c_str());
    if (!loaded) {
        std::cerr << "Failed to load texture '" << path
                  << "': " << SDL_GetError() << std::endl;
        return false;
    }

    // Получаем размеры текстуры
    float w = 0.0f, h = 0.0f;
    if (!SDL_GetTextureSize(loaded, &w, &h)) {
        std::cerr << "Failed to get texture size: " << SDL_GetError() << std::endl;
        SDL_DestroyTexture(loaded);
        return false;
    }

    m_texture = loaded;
    m_width   = (int)w;
    m_height  = (int)h;
    return true;
}

bool Texture::Create(SDL_Renderer* renderer, int width, int height,
                     SDL_PixelFormat format) {
    Destroy();

    SDL_Texture* created = SDL_CreateTexture(
        renderer, format, SDL_TEXTUREACCESS_TARGET, width, height);
    if (!created) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        return false;
    }

    m_texture = created;
    m_width   = width;
    m_height  = height;
    return true;
}

void Texture::Destroy() {
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
        m_width   = 0;
        m_height  = 0;
    }
}

// ===== Move-семантика =====

Texture::Texture(Texture&& other) noexcept
    : m_texture(other.m_texture)
    , m_width(other.m_width)
    , m_height(other.m_height)
{
    other.m_texture = nullptr;
    other.m_width   = 0;
    other.m_height  = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        Destroy();   // освобождаем свой текущий ресурс

        m_texture = other.m_texture;
        m_width   = other.m_width;
        m_height  = other.m_height;

        other.m_texture = nullptr;
        other.m_width   = 0;
        other.m_height  = 0;
    }
    return *this;
}