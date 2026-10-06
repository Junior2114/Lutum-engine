#pragma once

namespace m2d {

class Engine;
class Renderer;

class Game {
public:
    virtual ~Game() = default;

    virtual void OnInit(Engine& engine) = 0;
    virtual void OnUpdate(float dt) = 0;
    virtual void OnRender(Renderer& renderer) = 0;
    virtual void OnShutdown() {}
};

} // namespace m2d