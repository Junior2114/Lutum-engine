#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <map>
#include <memory>
#include "graphics/Texture.h"
#include "graphics/Font.h"

namespace m2d {

// Кеш текстур и шрифтов.
// Владеет ресурсами, возвращает сырые указатели без владения.
// Один и тот же ресурс загружается ровно один раз.
class ResourceManager {
public:
    ResourceManager() = default;
    ~ResourceManager() = default;

    ResourceManager(const ResourceManager&)            = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // ===== Текстуры =====

    // Загружает текстуру из файла, если её ещё нет в кеше.
    // Если уже загружена — возвращает указатель на существующую.
    // Возвращает nullptr при ошибке.
    Texture* GetTexture(SDL_Renderer* renderer, const std::string& path);

    // Удобный вариант — использует путь как имя.
    // path — относительный путь к файлу, например "assets/textures/player_sheet.png".
    Texture* GetTextureByPath(SDL_Renderer* renderer, const std::string& path);

    // ===== Шрифты =====

    // Загружает шрифт указанного размера.
    // Ключ кеша: path + ":" + size (один файл может использоваться в разных размерах).
    Font* GetFont(const std::string& path, float size);

    // ===== Управление =====

    // Освободить конкретный ресурс (редко нужно)
    void UnloadTexture(const std::string& name);
    void UnloadFont(const std::string& key);

    // Освободить всё (например, при смене сцены)
    void Clear();

    // Диагностика
    size_t GetTextureCount() const { return m_textures.size(); }
    size_t GetFontCount()    const { return m_fonts.size(); }

private:
    // unique_ptr обязателен: Texture и Font — move-only (нельзя копировать)
    std::map<std::string, std::unique_ptr<Texture>> m_textures;
    std::map<std::string, std::unique_ptr<Font>>    m_fonts;
};

} // namespace m2d