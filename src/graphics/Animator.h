#pragma once

#include <SDL3/SDL.h>
#include <map>
#include <string>
#include "graphics/Animation.h"

// "Проигрыватель" анимаций. Хранит набор анимаций по именам,
// знает текущую активную и обновляет её во времени.
class Animator {
public:
    // Добавить анимацию под именем (например, "walk_down")
    void Add(const std::string& name, Animation anim);

    // Начать проигрывать анимацию. Если она уже играет — ничего не делает.
    // reset=true принудительно начинает с нуля.
    void Play(const std::string& name, bool reset = false);

    // Обновить текущую анимацию на dt секунд
    void Update(float dt);

    // Получить прямоугольник текущего кадра для отрисовки.
    // Возвращает false, если анимации нет.
    bool GetCurrentFrame(SDL_FRect& outRect) const;

    // Удобно: узнать имя текущей анимации (для отладки)
    const std::string& GetCurrentName() const { return m_currentName; }

private:
    std::map<std::string, Animation> m_animations;

    std::string m_currentName;
    size_t m_currentFrame = 0;
    float  m_elapsed      = 0.0f;
};