#pragma once

#include <SDL3/SDL.h>
#include "core/Input.h"
#include "graphics/Texture.h"
#include "graphics/Renderer.h"
#include "graphics/Animator.h"

namespace m2d {

class Player {
public:
    Player() = default;

    void Init(Texture* sheet);
    void Update(const Input& input, float deltaTime);
    void Render(Renderer& renderer) const;

    float GetX() const { return m_x; }
    float GetY() const { return m_y; }

private:
    void SetupAnimations();

    enum class Facing { Down, Left, Right, Up };

    Texture* m_sheet = nullptr;
    Animator m_animator;

    float m_x = 100.0f;
    float m_y = 100.0f;
    Facing m_facing = Facing::Down;

    static constexpr float SPEED      = 200.0f;
    static constexpr int   FRAME_SIZE = 32;
    static constexpr float FRAME_TIME = 0.12f;
};

} // namespace m2d