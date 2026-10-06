#include "graphics/Animator.h"
#include "core/Log.h"

#include <utility>

namespace m2d {

void Animator::Add(const std::string& name, Animation anim) {
    m_animations[name] = std::move(anim);
}

void Animator::Play(const std::string& name, bool reset) {
    if (name == m_currentName && !reset) {
        return;
    }

    auto it = m_animations.find(name);
    if (it == m_animations.end()) {
        M2D_WARN("Animator: animation '", name, "' not found");
        return;
    }

    m_currentName  = name;
    m_currentAnim  = &it->second;
    m_currentFrame = 0;
    m_elapsed      = 0.0f;
}

void Animator::Update(float dt) {
    if (!m_currentAnim) return;

    const Animation& anim = *m_currentAnim;
    if (!anim.IsValid()) return;

    const float frameTime = anim.GetFrameTime();
    if (frameTime <= 0.0f) return;

    m_elapsed += dt;

    while (m_elapsed >= frameTime) {
        m_elapsed -= frameTime;

        const size_t frameCount = anim.GetFrameCount();
        if (m_currentFrame + 1 >= frameCount) {
            if (anim.IsLooping()) {
                m_currentFrame = 0;
            } else {
                m_currentFrame = frameCount - 1;
                m_elapsed      = 0.0f;
                break;
            }
        } else {
            ++m_currentFrame;
        }
    }
}

bool Animator::GetCurrentFrame(SDL_FRect& outRect) const {
    if (!m_currentAnim) return false;

    const Animation& anim = *m_currentAnim;
    if (!anim.IsValid()) return false;

    outRect = anim.GetFrame(m_currentFrame);
    return true;
}

} // namespace m2d