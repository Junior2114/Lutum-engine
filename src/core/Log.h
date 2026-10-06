#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

namespace m2d {

// Уровни важности сообщений
enum class LogLevel {
    Info,
    Warn,
    Error,
    None
};

class Log {
public:
    // Открыть лог-файл. Вызывается один раз при старте движка.
    // Если не вызвать — лог пишется только в консоль.
    static void Init(const std::string& filepath = "m2d.log");

    // Закрыть лог-файл.
    static void Shutdown();

    // Установить минимальный уровень. Сообщения ниже — игнорируются.
    static void SetLevel(LogLevel level) { s_minLevel = level; }

    // Записать сообщение заданного уровня.
    // Формат — как в std::ostringstream: пишем через <<.
    template<typename... Args>
    static void Write(LogLevel level, Args&&... args) {
        if (level < s_minLevel) return;

        // Формируем строку из всех аргументов через <<
        std::ostringstream oss;
        (oss << ... << args);
        WriteRaw(level, oss.str());
    }

private:
    static void WriteRaw(LogLevel level, const std::string& message);
    static const char* LevelToString(LogLevel level);
    static const char* LevelToColor(LogLevel level);

    static std::ofstream s_file;
    static bool          s_initialized;
    static LogLevel      s_minLevel;
};

} // namespace m2d

// ===== Удобные макросы =====
// Использование:
//   M2D_INFO("Loaded: ", path);
//   M2D_WARN("Font not found: ", path, ", size ", size);
//   M2D_ERROR("SDL failed: ", SDL_GetError());

#define M2D_INFO(...)  ::m2d::Log::Write(::m2d::LogLevel::Info,  __VA_ARGS__)
#define M2D_WARN(...)  ::m2d::Log::Write(::m2d::LogLevel::Warn,  __VA_ARGS__)
#define M2D_ERROR(...) ::m2d::Log::Write(::m2d::LogLevel::Error, __VA_ARGS__)

// ===== Assert =====
// В Debug (когда NDEBUG не определён):
//   - если условие ложно, пишет в лог, выводит в stderr и падает.
// В Release (NDEBUG определён):
//   - ничего не делает. Zero overhead.
//
// Использование:
//   M2D_ASSERT(texture != nullptr, "Texture must not be null");
//   M2D_ASSERT(m_renderer, "Renderer not initialized");

#ifdef NDEBUG
    #define M2D_ASSERT(condition, message) ((void)0)
#else
    #define M2D_ASSERT(condition, message)                              \
        do {                                                            \
            if (!(condition)) {                                         \
                ::m2d::Log::Write(::m2d::LogLevel::Error,               \
                    "[ASSERT] ", message,                               \
                    " (" #condition ")",                                \
                    " at ", __FILE__, ":", __LINE__);                   \
                std::cerr << "[ASSERT] " << message                     \
                          << " (" #condition ")"                        \
                          << " at " << __FILE__ << ":" << __LINE__      \
                          << std::endl;                                 \
                std::abort();                                           \
            }                                                           \
        } while (0)
#endif