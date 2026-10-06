#pragma once

#include <memory>
#include <vector>
#include "core/Scene.h"

namespace m2d {

class Engine;
class Renderer;

class SceneManager {
public:
    SceneManager() = default;
    ~SceneManager() = default;

    SceneManager(const SceneManager&)            = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    // ===== Управление стеком =====

    // Кладёт сцену поверх всех. Текущая остаётся в стеке (для паузы).
    void Push(std::unique_ptr<Scene> scene, Engine& engine);

    // Убирает верхнюю сцену. Если стек пуст — ничего не делает.
    void Pop();

    // Заменяет ВСЕ сцены на одну. Используется для смены уровней.
    void Replace(std::unique_ptr<Scene> scene, Engine& engine);

    // Очищает весь стек.
    void Clear();

    // ===== Цикл =====

    // Вызывает OnUpdate у верхней сцены. Обрабатывает отложенное удаление.
    void Update(float dt);

    // Вызывает OnRender у ВСЕХ сцен, снизу вверх.
    void Render(Renderer& renderer);

    // Пробрасывает SDL-событие верхней сцене.
    void HandleEvent(const SDL_Event& event);

    // ===== Состояние =====

    bool   IsEmpty() const { return m_scenes.empty(); }
    size_t Size()    const { return m_scenes.size(); }

    Scene* GetTop() {
        return m_scenes.empty() ? nullptr : m_scenes.back().get();
    }

private:
    // Убирает сцены, которые запросили удаление
    void ProcessPendingRemovals();

    std::vector<std::unique_ptr<Scene>> m_scenes;
};

} // namespace m2d