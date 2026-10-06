#include "graphics/ResourceManager.h"
#include "core/Log.h"

namespace m2d {

Texture* ResourceManager::GetTexture(SDL_Renderer* renderer,
                                     const std::string& path) {
    auto it = m_textures.find(path);
    if (it != m_textures.end()) {
        return it->second.get();
    }

    auto texture = std::make_unique<Texture>();
    if (!texture->LoadFromFile(renderer, path)) {
        M2D_ERROR("Failed to load texture: ", path);
        return nullptr;
    }

    Texture* raw = texture.get();
    m_textures[path] = std::move(texture);

    M2D_INFO("Loaded texture: ", path);
    return raw;
}

Texture* ResourceManager::GetTextureByPath(SDL_Renderer* renderer,
                                           const std::string& path) {
    return GetTexture(renderer, path);
}

Font* ResourceManager::GetFont(const std::string& path, float size) {
    const std::string key = path + ":" + std::to_string(size);

    auto it = m_fonts.find(key);
    if (it != m_fonts.end()) {
        return it->second.get();
    }

    auto font = std::make_unique<Font>();
    if (!font->LoadFromFile(path, size)) {
        M2D_ERROR("Failed to load font: ", path, " (size ", size, ")");
        return nullptr;
    }

    Font* raw = font.get();
    m_fonts[key] = std::move(font);

    M2D_INFO("Loaded font: ", path, " (size ", size, ")");
    return raw;
}

void ResourceManager::UnloadTexture(const std::string& name) {
    m_textures.erase(name);
}

void ResourceManager::UnloadFont(const std::string& key) {
    m_fonts.erase(key);
}

void ResourceManager::Clear() {
    m_textures.clear();
    m_fonts.clear();
    M2D_INFO("Cleared all resources");
}

} // namespace m2d