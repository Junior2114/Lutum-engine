#pragma once

#include "core/Game.h"
#include "core/Engine.h"
#include "core/SceneManager.h"

namespace mygame {

class MyGame : public m2d::Game {
public:
    void OnInit(m2d::Engine& engine) override;
    void OnUpdate(float dt) override;
    void OnRender(m2d::Renderer& renderer) override;
    void OnShutdown() override;

    // Игра даёт сценам доступ к менеджеру сцен
    m2d::SceneManager& GetSceneManager() { return m_sceneManager; }

private:
    m2d::Engine*       m_engine = nullptr;
    m2d::SceneManager  m_sceneManager;
};

} // namespace mygame