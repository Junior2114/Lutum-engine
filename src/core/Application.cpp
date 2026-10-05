#include "core/Application.h"

#include <iostream>
#include <string>

Application::Application() {}

Application::~Application() {
    Shutdown();
}

// ===== Настройка анимаций =====
// Спрайт-лист: 4 строки x 4 кадра. Размер кадра 32x32.
//   строка 0: вниз   (красный)
//   строка 1: влево  (синий)
//   строка 2: вправо (зелёный)
//   строка 3: вверх  (жёлтый)
void Application::SetupAnimations() {
    const float fw = (float)FRAME_SIZE;
    const float fh = (float)FRAME_SIZE;
    const float frameTime = 0.12f;   // ~8 FPS анимации

    Animation walkDown, walkLeft, walkRight, walkUp;
    walkDown .AddFramesFromRow(fw, fh, 0, 0, 4);
    walkLeft .AddFramesFromRow(fw, fh, 1, 0, 4);
    walkRight.AddFramesFromRow(fw, fh, 2, 0, 4);
    walkUp   .AddFramesFromRow(fw, fh, 3, 0, 4);

    walkDown .SetFrameTime(frameTime);
    walkLeft .SetFrameTime(frameTime);
    walkRight.SetFrameTime(frameTime);
    walkUp   .SetFrameTime(frameTime);

    m_animator.Add("walk_down",  std::move(walkDown));
    m_animator.Add("walk_left",  std::move(walkLeft));
    m_animator.Add("walk_right", std::move(walkRight));
    m_animator.Add("walk_up",    std::move(walkUp));

    // По умолчанию — «стоим» вниз (первый кадр)
    m_animator.Play("walk_down", true);
}

bool Application::Init(const char* title, int width, int height) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!SDL_CreateWindowAndRenderer(title, width, height, 0,
                                     &m_window, &m_renderer)) {
        std::cerr << "Failed to create window/renderer: "
                  << SDL_GetError() << std::endl;
        return false;
    }

    m_rendererWrap = Renderer(m_renderer);

    // Загружаем спрайт-лист (128x128)
    if (!m_playerSheet.LoadFromFile(m_renderer,
                                    "assets/textures/player_sheet.png")) {
        std::cerr << "Warning: player_sheet.png not loaded." << std::endl;
    } else {
        std::cout << "Loaded player_sheet.png ("
                  << m_playerSheet.GetWidth() << "x"
                  << m_playerSheet.GetHeight() << ")" << std::endl;
    }

    SetupAnimations();

    m_frequency   = (double)SDL_GetPerformanceFrequency();
    m_lastCounter = SDL_GetPerformanceCounter();
    m_running     = true;

    std::cout << "Engine initialized: " << title
              << " (" << width << "x" << height << ")" << std::endl;
    return true;
}

void Application::Run() {
    while (m_running) {
        Uint64 now = SDL_GetPerformanceCounter();
        float dt = (float)((now - m_lastCounter) / m_frequency);
        m_lastCounter = now;

        // Ограничение dt — если окно перетаскивали, dt может стать огромным
        // и сломать анимацию. 0.1 сек = 100 мс — разумный потолок.
        if (dt > 0.1f) dt = 0.1f;

        m_input.BeginFrame();
        PollEvents();
        Update(dt);
        Render();
    }
}

void Application::PollEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        m_input.ProcessEvent(event);
        if (event.type == SDL_EVENT_QUIT) {
            m_running = false;
        }
    }
}

void Application::Update(float deltaTime) {
    if (m_input.IsKeyPressed(SDL_SCANCODE_ESCAPE)) {
        m_running = false;
    }

    // ===== Определяем направление движения =====
    float dx = 0.0f;
    float dy = 0.0f;

    if (m_input.IsKeyDown(SDL_SCANCODE_W) || m_input.IsKeyDown(SDL_SCANCODE_UP))
        dy -= 1.0f;
    if (m_input.IsKeyDown(SDL_SCANCODE_S) || m_input.IsKeyDown(SDL_SCANCODE_DOWN))
        dy += 1.0f;
    if (m_input.IsKeyDown(SDL_SCANCODE_A) || m_input.IsKeyDown(SDL_SCANCODE_LEFT))
        dx -= 1.0f;
    if (m_input.IsKeyDown(SDL_SCANCODE_D) || m_input.IsKeyDown(SDL_SCANCODE_RIGHT))
        dx += 1.0f;

    const bool moving = (dx != 0.0f || dy != 0.0f);

    // Нормализация диагонали
    if (dx != 0.0f && dy != 0.0f) {
        const float inv = 1.0f / 1.41421356f;
        dx *= inv;
        dy *= inv;
    }

    m_playerX += dx * PLAYER_SPEED * deltaTime;
    m_playerY += dy * PLAYER_SPEED * deltaTime;

    // ===== Выбор анимации по направлению =====
    if (moving) {
        // Приоритет: горизонталь важнее вертикали (типичное поведение)
        if      (dx < 0.0f) m_animator.Play("walk_left");
        else if (dx > 0.0f) m_animator.Play("walk_right");
        else if (dy < 0.0f) m_animator.Play("walk_up");
        else if (dy > 0.0f) m_animator.Play("walk_down");
    }
    // Если не движемся — оставляем последнюю активную анимацию,
    // но останавливаем её на первом кадре. Простейший idle.

    // Обновляем анимацию
    m_animator.Update(deltaTime);

    // Заголовок окна — отладка
    static float timer = 0.0f;
    timer += deltaTime;
    if (timer >= 0.1f) {
        timer = 0.0f;
        std::string title = "Player: " +
            std::to_string((int)m_playerX) + ", " +
            std::to_string((int)m_playerY) +
            "  [" + m_animator.GetCurrentName() + "]";
        SDL_SetWindowTitle(m_window, title.c_str());
    }
}

void Application::Render() {
    m_rendererWrap.Clear(30, 30, 30);

    // Получаем текущий кадр анимации и рисуем его
    SDL_FRect srcRect;
    if (m_animator.GetCurrentFrame(srcRect)) {
        SDL_FRect dstRect{
            m_playerX,
            m_playerY,
            (float)FRAME_SIZE,
            (float)FRAME_SIZE
        };
        m_rendererWrap.DrawTextureRegion(m_playerSheet, srcRect, dstRect);
    }

    m_rendererWrap.Present();
}

void Application::Shutdown() {
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    if (m_running || m_frequency > 0.0) {
        SDL_Quit();
        m_frequency = 0.0;
    }
    m_running = false;
}