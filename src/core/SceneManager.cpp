#include "core/SceneManager.h"
#include "core/Engine.h"
#include "graphics/Renderer.h"

#include <algorithm>

namespace m2d {

// ===== Управление стеком =====

void SceneManager::Push(std::unique_ptr<Scene> scene, Engine& engine) {
    if (!scene) {
        M2D_ERROR("SceneManager::Push called with null scene");
        return;
    }

    scene->OnEnter(engine);
    m_scenes.push_back(std::move(scene));

    M2D_INFO("Scene pushed (stack size: ", m_scenes.size(), ")");
}

void SceneManager::Pop() {
    if (m_scenes.empty()) {
        M2D_WARN("SceneManager::Pop called on empty stack");
        return;
    }

    m_scenes.back()->OnExit();
    m_scenes.pop_back();

    M2D_INFO("Scene popped (stack size: ", m_scenes.size(), ")");
}

void SceneManager::Replace(std::unique_ptr<Scene> scene, Engine& engine) {
    Clear();
    Push(std::move(scene), engine);
}

void SceneManager::Clear() {
    // Вызываем OnExit в обратном порядке — как в стеке
    for (auto it = m_scenes.rbegin(); it != m_scenes.rend(); ++it) {
        (*it)->OnExit();
    }
    m_scenes.clear();
    M2D_INFO("SceneManager cleared");
}

// ===== Цикл =====

void SceneManager::Update(float dt) {
    // OnUpdate — только для верхней сцены
    if (!m_scenes.empty()) {
        m_scenes.back()->OnUpdate(dt);
    }

    // Обрабатываем сцены, которые запросили удаление
    ProcessPendingRemovals();
}

void SceneManager::Render(Renderer& renderer) {
    // Отрисовываем ВСЕ сцены, снизу вверх.
    // Так PauseScene может рисоваться поверх PlayScene.
    for (auto& scene : m_scenes) {
        scene->OnRender(renderer);
    }
}

void SceneManager::HandleEvent(const SDL_Event& event) {
    // Событие — только верхней сцене
    if (!m_scenes.empty()) {
        m_scenes.back()->OnEvent(event);
    }
}

// ===== Внутреннее =====

void SceneManager::ProcessPendingRemovals() {
    // Удаляем сцены, которые запросили removal.
    // Идём с конца — если верхняя просит удаления, удаляем её.
    // Если сцена в середине — тоже удаляется, но это редко.
    auto it = std::remove_if(m_scenes.begin(), m_scenes.end(),
        [](const std::unique_ptr<Scene>& scene) {
            if (scene->IsPendingRemoval()) {
                scene->OnExit();
                return true;
            }
            return false;
        });

    if (it != m_scenes.end()) {
        m_scenes.erase(it, m_scenes.end());
        M2D_INFO("Removed pending scenes (stack size: ", m_scenes.size(), ")");
    }
}

} // namespace m2d