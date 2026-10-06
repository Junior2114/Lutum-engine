#pragma once

#include "core/Game.h"
#include "core/Engine.h"
#include "graphics/Texture.h"
#include "graphics/Font.h"
#include "game/Player.h"

namespace mygame {

class MyGame : public m2d::Game {
public:
    void OnInit(m2d::Engine& engine) override;
    void OnUpdate(float dt) override;
    void OnRender(m2d::Renderer& renderer) override;
    void OnShutdown() override;

private:
    m2d::Engine* m_engine = nullptr;
    m2d::Input*  m_input  = nullptr;

    m2d::Texture m_playerSheet;
    m2d::Font    m_font;
    m2d::Player  m_player;

    int   m_fps        = 0;
    int   m_frameCount = 0;
    float m_fpsTimer   = 0.0f;
};

} // namespace mygame