#include "graphics/DebugOverlay.h"

#include <string>
#include <sstream>
#include <iomanip>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <windows.h>
    #include <psapi.h>
#endif

namespace m2d {

void DebugOverlay::Init(Font* font) {
    m_font = font;
}

void DebugOverlay::Update(float dt) {
    m_fpsTimer   += dt;
    m_frameCount += 1;

    const float instantFrameMs = dt * 1000.0f;
    m_frameTimeMs = m_frameTimeMs * 0.9f + instantFrameMs * 0.1f;

    if (m_fpsTimer >= 1.0f) {
        m_fps        = m_frameCount;
        m_frameCount = 0;
        m_fpsTimer  -= 1.0f;
    }

    UpdatePlatformMetrics(dt);
}

void DebugOverlay::UpdatePlatformMetrics(float dt) {
    (void)dt;

#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS pmc{};
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        m_ramMB = (float)pmc.WorkingSetSize / (1024.0f * 1024.0f);
    }

    static ULONGLONG lastProcessTime = 0;
    static ULONGLONG lastRealTime    = 0;

    FILETIME creation, exit, kernel, user;
    if (GetProcessTimes(GetCurrentProcess(), &creation, &exit,
                        &kernel, &user)) {
        ULARGE_INTEGER k{}, u{};
        k.LowPart  = kernel.dwLowDateTime;
        k.HighPart = kernel.dwHighDateTime;
        u.LowPart  = user.dwLowDateTime;
        u.HighPart = user.dwHighDateTime;

        ULONGLONG processTime = k.QuadPart + u.QuadPart;

        ULARGE_INTEGER now{};
        GetSystemTimeAsFileTime(reinterpret_cast<FILETIME*>(&now));

        if (lastProcessTime != 0 && now.QuadPart > lastRealTime) {
            ULONGLONG procDelta = processTime - lastProcessTime;
            ULONGLONG realDelta = now.QuadPart - lastRealTime;

            if (realDelta > 0) {
                float raw = (float)((double)procDelta * 100.0
                                    / (double)realDelta);

                SYSTEM_INFO si{};
                GetSystemInfo(&si);
                int cores = (int)si.dwNumberOfProcessors;
                if (cores > 0) raw /= (float)cores;

                m_cpuPercent = m_cpuPercent * 0.8f + raw * 0.2f;
            }
        }

        lastProcessTime = processTime;
        lastRealTime    = now.QuadPart;
    }
#endif
}

void DebugOverlay::Render(Renderer& renderer,
                          int windowWidth, int windowHeight) {
    if (!m_font) return;

    const float panelHeight = 32.0f;
    const float panelY      = (float)windowHeight - panelHeight;

    SDL_FRect panel{ 0.0f, panelY, (float)windowWidth, panelHeight };
    renderer.DrawRect(panel, 15, 15, 20, 230);

    SDL_FRect separator{ 0.0f, panelY, (float)windowWidth, 1.0f };
    renderer.DrawRect(separator, 70, 70, 90, 255);

    const float textY = panelY + 6.0f;
    const Uint8 r = 200, g = 200, b = 200;

    std::ostringstream ss;
    ss << std::fixed << std::setprecision(1);

    float x = 15.0f;

    ss.str(""); ss.clear();
    ss << "FPS: " << m_fps;
    x += renderer.DrawString(*m_font, ss.str(), x, textY, r, g, b);
    x += 30.0f;

    ss.str(""); ss.clear();
    ss << "Frame: " << m_frameTimeMs << " ms";
    x += renderer.DrawString(*m_font, ss.str(), x, textY, r, g, b);
    x += 30.0f;

    Uint8 cpuR = 100, cpuG = 220, cpuB = 100;
    if (m_cpuPercent > 25.0f) { cpuR = 230; cpuG = 200; cpuB = 80; }
    if (m_cpuPercent > 60.0f) { cpuR = 230; cpuG = 100; cpuB = 100; }

    ss.str(""); ss.clear();
    ss << "CPU: " << m_cpuPercent << " %";
    x += renderer.DrawString(*m_font, ss.str(), x, textY, cpuR, cpuG, cpuB);
    x += 30.0f;

    ss.str(""); ss.clear();
    ss << "RAM: " << m_ramMB << " MB";
    renderer.DrawString(*m_font, ss.str(), x, textY, 130, 180, 240);
}

} // namespace m2d