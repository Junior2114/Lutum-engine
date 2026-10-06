#include "game/Player.h"

#include <cmath>
#include <utility>

namespace m2d {

void Player::Init(Texture* sheet) {
    m_sheet = sheet;
    SetupAnimations();
    m_animator.Play("idle_down", true);
}

void Player::SetupAnimations() {
    const float fw = (float)FRAME_SIZE;
    const float fh = (float)FRAME_SIZE;

    Animation walkDown, walkLeft, walkRight, walkUp;
    walkDown .AddFramesFromRow(fw, fh, 0, 0, 4);
    walkLeft .AddFramesFromRow(fw, fh, 1, 0, 4);
    walkRight.AddFramesFromRow(fw, fh, 2, 0, 4);
    walkUp   .AddFramesFromRow(fw, fh, 3, 0, 4);

    walkDown .SetFrameTime(FRAME_TIME);
    walkLeft .SetFrameTime(FRAME_TIME);
    walkRight.SetFrameTime(FRAME_TIME);
    walkUp   .SetFrameTime(FRAME_TIME);

    m_animator.Add("walk_down",  std::move(walkDown));
    m_animator.Add("walk_left",  std::move(walkLeft));
    m_animator.Add("walk_right", std::move(walkRight));
    m_animator.Add("walk_up",    std::move(walkUp));

    Animation idleDown, idleLeft, idleRight, idleUp;
    idleDown .AddFramesFromRow(fw, fh, 0, 0, 1);
    idleLeft .AddFramesFromRow(fw, fh, 1, 0, 1);
    idleRight.AddFramesFromRow(fw, fh, 2, 0, 1);
    idleUp   .AddFramesFromRow(fw, fh, 3, 0, 1);

    idleDown .SetFrameTime(1000.0f);
    idleLeft .SetFrameTime(1000.0f);
    idleRight.SetFrameTime(1000.0f);
    idleUp   .SetFrameTime(1000.0f);

    m_animator.Add("idle_down",  std::move(idleDown));
    m_animator.Add("idle_left",  std::move(idleLeft));
    m_animator.Add("idle_right", std::move(idleRight));
    m_animator.Add("idle_up",    std::move(idleUp));
}

void Player::Update(const Input& input, float deltaTime) {
    float dx = 0.0f;
    float dy = 0.0f;

    if (input.IsKeyDown(SDL_SCANCODE_W) || input.IsKeyDown(SDL_SCANCODE_UP))
        dy -= 1.0f;
    if (input.IsKeyDown(SDL_SCANCODE_S) || input.IsKeyDown(SDL_SCANCODE_DOWN))
        dy += 1.0f;
    if (input.IsKeyDown(SDL_SCANCODE_A) || input.IsKeyDown(SDL_SCANCODE_LEFT))
        dx -= 1.0f;
    if (input.IsKeyDown(SDL_SCANCODE_D) || input.IsKeyDown(SDL_SCANCODE_RIGHT))
        dx += 1.0f;

    const bool moving = (dx != 0.0f || dy != 0.0f);

    if (dx != 0.0f && dy != 0.0f) {
        constexpr float INV_SQRT2 = 0.70710678f;
        dx *= INV_SQRT2;
        dy *= INV_SQRT2;
    }

    m_x += dx * SPEED * deltaTime;
    m_y += dy * SPEED * deltaTime;

    if (moving) {
        if (std::abs(dx) > std::abs(dy)) {
            m_facing = (dx < 0.0f) ? Facing::Left : Facing::Right;
        } else {
            m_facing = (dy < 0.0f) ? Facing::Up : Facing::Down;
        }

        switch (m_facing) {
            case Facing::Down:  m_animator.Play("walk_down");  break;
            case Facing::Left:  m_animator.Play("walk_left");  break;
            case Facing::Right: m_animator.Play("walk_right"); break;
            case Facing::Up:    m_animator.Play("walk_up");    break;
        }
    } else {
        switch (m_facing) {
            case Facing::Down:  m_animator.Play("idle_down");  break;
            case Facing::Left:  m_animator.Play("idle_left");  break;
            case Facing::Right: m_animator.Play("idle_right"); break;
            case Facing::Up:    m_animator.Play("idle_up");    break;
        }
    }

    m_animator.Update(deltaTime);
}

void Player::Render(Renderer& renderer) const {
    if (!m_sheet) return;

    SDL_FRect srcRect;
    if (!m_animator.GetCurrentFrame(srcRect)) return;

    SDL_FRect dstRect{
        m_x,
        m_y,
        (float)FRAME_SIZE,
        (float)FRAME_SIZE
    };

    renderer.DrawTextureRegion(*m_sheet, srcRect, dstRect);
}

} // namespace m2d