#pragma once

#include "core/Scene.h"
#include "core/Engine.h"
#include "ui/UIPanel.h"
#include "ui/UILabel.h"
#include "ui/UIButton.h"
#include "graphics/Texture.h"
#include "graphics/Font.h"
#include "game/Player.h"

namespace mygame {

class MyGame;

class PlayScene : public m2d::Scene {
public:
    explicit PlayScene(MyGame* game) : m_game(game) {}

    void OnEnter(m2d::Engine& engine) override;
    void OnExit() override;
    void OnUpdate(float dt) override;
    void OnRender(m2d::Renderer& renderer) override;

private:
    MyGame* m_game = nullptr;
    m2d::Engine* m_engine = nullptr;
    m2d::Input*  m_input  = nullptr;

    m2d::Texture* m_playerSheet = nullptr;
    m2d::Font*    m_font        = nullptr;

    m2d::Player   m_player;
};

} // namespace mygame