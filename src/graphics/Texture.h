#pragma once

#include <SDL3/SDL.h>
#include <string>

// RAII-обёртка над SDL_Texture*.
// Владеет текстурой и автоматически освобождает её в деструкторе.
// Копирование запрещено, перемещение разрешено.
class Texture {
public:
    Texture() = default;
    ~Texture();

    // Загружает PNG/JPG с диска в GPU. Возвращает false при ошибке.
    bool LoadFromFile(SDL_Renderer* renderer, const std::string& path);

    // Создаёт пустую текстуру заданного размера (например, для render target)
    bool Create(SDL_Renderer* renderer, int width, int height,
                SDL_PixelFormat format = SDL_PIXELFORMAT_RGBA8888);

    // Освобождает текстуру досрочно (не обязательно — деструктор сделает это)
    void Destroy();

    // Доступ к сырому указателю для SDL API
    SDL_Texture* Get() const { return m_texture; }

    int GetWidth()  const { return m_width; }
    int GetHeight() const { return m_height; }

    bool IsValid() const { return m_texture != nullptr; }

    // Копирование запрещено — иначе двойное освобождение
    Texture(const Texture&)            = delete;
    Texture& operator=(const Texture&) = delete;

    // Перемещение разрешено — владение передаётся
    Texture(Texture&& other) noexcept;
    Texture& operator=(Texture&& other) noexcept;

private:
    SDL_Texture* m_texture = nullptr;
    int m_width  = 0;
    int m_height = 0;
};