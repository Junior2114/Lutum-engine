#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

namespace m2d {

class Font {
public:
    Font() = default;
    ~Font();

    bool LoadFromFile(const std::string& path, float pointSize);
    void Destroy();

    TTF_Font* Get() const { return m_font; }
    bool IsValid() const { return m_font != nullptr; }

    Font(const Font&)            = delete;
    Font& operator=(const Font&) = delete;
    Font(Font&& other) noexcept;
    Font& operator=(Font&& other) noexcept;

private:
    TTF_Font* m_font = nullptr;
};

} // namespace m2d