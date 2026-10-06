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

    // Не владеющие указатели — ресурсы принадлежат Engine::m_resources
    m2d::Texture* m_playerSheet = nullptr;
    m2d::Font*    m_font        = nullptr;

    m2d::Player   m_player;
};

} // namespace mygame