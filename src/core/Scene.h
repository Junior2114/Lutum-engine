#pragma once

#include <SDL3/SDL.h>       // <-- ЭТА СТРОКА БЫЛА ПРОПУЩЕНА
#include "core/Log.h"

namespace m2d {

class Engine;
class Renderer;

// Базовый класс для любой сцены: меню, уровень, пауза, game over.
class Scene {
public:
    virtual ~Scene() = default;

    virtual void OnEnter(Engine& engine) = 0;
    virtual void OnExit() {}
    virtual void OnUpdate(float dt) = 0;
    virtual void OnRender(Renderer& renderer) = 0;

    // Опционально: обработка SDL-события (требует полного типа SDL_Event)
    virtual void OnEvent(const SDL_Event& /*event*/) {}

    bool IsPendingRemoval() const { return m_pendingRemoval; }
    void RequestRemoval()          { m_pendingRemoval = true; }

private:
    bool m_pendingRemoval = false;
};

} // namespace m2d