#pragma once

#include <SDL3/SDL.h>
#include "core/Input.h"
#include "core/Log.h"
#include "ui/UILayer.h"            // <-- новое
#include "graphics/Renderer.h"
#include "graphics/DebugOverlay.h"
#include "graphics/ResourceManager.h"

namespace m2d {

class Game;

class Engine {
public:
    Engine();
    ~Engine();

    bool Init(const char* title, int width, int height);
    void Run(Game& game);
    void Shutdown();

    Input&           GetInput()         { return m_input; }
    Renderer&        GetRenderer()      { return m_rendererWrap; }
    ResourceManager& GetResources()     { return m_resources; }
    DebugOverlay&    GetDebugOverlay()  { return m_debugOverlay; }
    UILayer&         GetUI()            { return m_ui; }             // <-- новое

    SDL_Window*   GetWindow()      { return m_window; }
    SDL_Renderer* GetSDLRenderer() { return m_renderer; }

    int GetWidth()  const { return m_width; }
    int GetHeight() const { return m_height; }

    void SetDebugOverlayVisible(bool v) { m_showDebugOverlay = v; }
    bool IsDebugOverlayVisible() const  { return m_showDebugOverlay; }
    void ToggleDebugOverlay()           { m_showDebugOverlay = !m_showDebugOverlay; }

    void RequestQuit() { m_running = false; }

private:
    void PollEvents();

private:
    SDL_Window*   m_window   = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    bool          m_running  = false;

    int m_width  = 0;
    int m_height = 0;

    Uint64 m_lastCounter = 0;
    double m_frequency   = 0.0;

    Input           m_input;
    Renderer        m_rendererWrap;
    ResourceManager m_resources;
    UILayer         m_ui;                 // <-- новое
    DebugOverlay    m_debugOverlay;
    bool            m_showDebugOverlay = true;
};

} // namespace m2d