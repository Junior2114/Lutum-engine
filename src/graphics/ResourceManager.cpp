#include "graphics/ResourceManager.h"

#include <iostream>

namespace m2d {

// ===== Текстуры =====

Texture* ResourceManager::GetTexture(SDL_Renderer* renderer,
                                     const std::string& path) {
    // Ключ — полный путь. Один и тот же файл — один и тот же ключ.
    auto it = m_textures.find(path);
    if (it != m_textures.end()) {
        return it->second.get();
    }

    // Загружаем
    auto texture = std::make_unique<Texture>();
    if (!texture->LoadFromFile(renderer, path)) {
        std::cerr << "[ResourceManager] Failed to load texture: "
                  << path << std::endl;
        return nullptr;
    }

    Texture* raw = texture.get();
    m_textures[path] = std::move(texture);

    std::cout << "[ResourceManager] Loaded texture: " << path << std::endl;
    return raw;
}

Texture* ResourceManager::GetTextureByPath(SDL_Renderer* renderer,
                                           const std::string& path) {
    return GetTexture(renderer, path);
}

// ===== Шрифты =====

Font* ResourceManager::GetFont(const std::string& path, float size) {
    // Ключ: path + ":" + size — один файл может быть в разных размерах
    const std::string key = path + ":" + std::to_string(size);

    auto it = m_fonts.find(key);
    if (it != m_fonts.end()) {
        return it->second.get();
    }

    auto font = std::make_unique<Font>();
    if (!font->LoadFromFile(path, size)) {
        std::cerr << "[ResourceManager] Failed to load font: "
                  << path << " (size " << size << ")" << std::endl;
        return nullptr;
    }

    Font* raw = font.get();
    m_fonts[key] = std::move(font);

    std::cout << "[ResourceManager] Loaded font: " << path
              << " (size " << size << ")" << std::endl;
    return raw;
}

// ===== Управление =====

void ResourceManager::UnloadTexture(const std::string& name) {
    m_textures.erase(name);
}

void ResourceManager::UnloadFont(const std::string& key) {
    m_fonts.erase(key);
}

void ResourceManager::Clear() {
    m_textures.clear();
    m_fonts.clear();
    std::cout << "[ResourceManager] Cleared all resources" << std::endl;
}

} // namespace m2d