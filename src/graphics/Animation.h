#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include <string>

// Данные ОДНОЙ анимации: набор кадров + скорость + зацикленность.
// Сам по себе не обновляется — это делает Animator.
class Animation {
public:
    Animation() = default;

    // Добавить кадр — прямоугольник в спрайт-листе
    void AddFrame(const SDL_FRect& frame);

    // Нарезать спрайт-лист в сетку и добавить выбранные кадры
    // row, colStart..colEnd — с какого по какой столбец в строке row
    void AddFramesFromRow(float frameW, float frameH,
                          int row, int colStart, int colCount);

    void SetFrameTime(float seconds) { m_frameTime = seconds; }
    void SetLooping(bool looping)    { m_looping = looping; }

    float GetFrameTime() const { return m_frameTime; }
    bool  IsLooping()    const { return m_looping; }

    const SDL_FRect& GetFrame(size_t index) const;
    size_t GetFrameCount() const { return m_frames.size(); }

    bool IsValid() const { return !m_frames.empty(); }

private:
    std::vector<SDL_FRect> m_frames;
    float m_frameTime = 0.1f;   // секунд на кадр по умолчанию
    bool  m_looping   = true;
};