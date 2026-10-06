#pragma once

#include <SDL3/SDL.h>
#include <map>
#include <string>
#include "graphics/Animation.h"

namespace m2d {

class Animator {
public:
    void Add(const std::string& name, Animation anim);

    void Play(const std::string& name, bool reset = false);

    void Update(float dt);

    bool GetCurrentFrame(SDL_FRect& outRect) const;

    const std::string& GetCurrentName() const { return m_currentName; }

private:
    std::map<std::string, Animation> m_animations;

    std::string m_currentName;
    size_t m_currentFrame = 0;
    float  m_elapsed      = 0.0f;
};

} // namespace m2d