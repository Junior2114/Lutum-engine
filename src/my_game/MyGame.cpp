#include "my_game/MyGame.h"
#include "my_game/PlayScene.h"
#include "core/Log.h"

namespace mygame {

void MyGame::OnInit(m2d::Engine& engine) {
    m_engine = &engine;

    // Создаём первую сцену — игровую.
    // Позже здесь можно сделать MenuScene и переключиться на неё.
    m_sceneManager.Push(std::make_unique<PlayScene>(this), engine);

    M2D_INFO("MyGame initialized");
}

void MyGame::OnUpdate(float dt) {
    m_sceneManager.Update(dt);
}

void MyGame::OnRender(m2d::Renderer& renderer) {
    m_sceneManager.Render(renderer);
}

void MyGame::OnShutdown() {
    m_sceneManager.Clear();
    M2D_INFO("MyGame shutdown");
}

} // namespace mygame