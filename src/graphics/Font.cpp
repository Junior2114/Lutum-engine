#include "graphics/Font.h"
#include <iostream>

namespace m2d {

Font::~Font() {
    Destroy();
}

bool Font::LoadFromFile(const std::string& path, float pointSize) {
    Destroy();

    TTF_Font* loaded = TTF_OpenFont(path.c_str(), pointSize);
    if (!loaded) {
        std::cerr << "Failed to load font '" << path
                  << "': " << SDL_GetError() << std::endl;
        return false;
    }

    m_font = loaded;
    return true;
}

void Font::Destroy() {
    if (m_font) {
        TTF_CloseFont(m_font);
        m_font = nullptr;
    }
}

Font::Font(Font&& other) noexcept
    : m_font(other.m_font)
{
    other.m_font = nullptr;
}

Font& Font::operator=(Font&& other) noexcept {
    if (this != &other) {
        Destroy();
        m_font = other.m_font;
        other.m_font = nullptr;
    }
    return *this;
}

} // namespace m2d