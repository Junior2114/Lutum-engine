#include "graphics/Animation.h"

void Animation::AddFrame(const SDL_FRect& frame) {
    m_frames.push_back(frame);
}

void Animation::AddFramesFromRow(float frameW, float frameH,
                                 int row, int colStart, int colCount) {
    for (int i = 0; i < colCount; ++i) {
        int col = colStart + i;
        SDL_FRect rect{
            col * frameW,      // x в спрайт-листе
            row * frameH,      // y в спрайт-листе
            frameW,
            frameH
        };
        m_frames.push_back(rect);
    }
}

const SDL_FRect& Animation::GetFrame(size_t index) const {
    // Безопасный доступ: если индекс вышел за пределы — вернём последний кадр
    if (index >= m_frames.size()) {
        return m_frames.back();
    }
    return m_frames[index];
}