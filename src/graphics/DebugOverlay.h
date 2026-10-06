#pragma once

#include <SDL3/SDL.h>
#include "graphics/Renderer.h"
#include "graphics/Font.h"

namespace m2d {

// Отладочный HUD внизу экрана: FPS, frame time, CPU, RAM.
// Собирает метрики сам, рисует через переданный Renderer.
class DebugOverlay {
public:
    DebugOverlay() = default;

    // font — уже загруженный шрифт. Владение остаётся у вызывающего.
    void Init(Font* font);

    // Вызывать каждый кадр в OnUpdate. Здесь считаются метрики.
    void Update(float dt);

    // Вызывать каждый кадр в OnRender. Рисует панель внизу окна.
    void Render(Renderer& renderer, int windowWidth, int windowHeight);

    // Геттеры — если игра хочет показать что-то своё
    int   GetFPS()        const { return m_fps; }
    float GetFrameTime()  const { return m_frameTimeMs; }
    float GetCpuUsage()   const { return m_cpuPercent; }
    float GetRamUsageMB() const { return m_ramMB; }

private:
    // Обновление CPU/RAM — Windows-only, на других платформах no-op
    void UpdatePlatformMetrics(float dt);

    Font* m_font = nullptr;

    // ===== FPS / frame time =====
    int   m_fps         = 0;
    int   m_frameCount  = 0;
    float m_fpsTimer    = 0.0f;
    float m_frameTimeMs = 0.0f;

    // ===== CPU (Windows) =====
    float m_cpuPercent = 0.0f;

    // ===== RAM (Windows) =====
    float m_ramMB = 0.0f;
};

} // namespace m2d