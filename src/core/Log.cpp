#include "core/Log.h"
#include <chrono>
#include <iomanip>
#include <ctime>

#ifdef _WIN32
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #include <windows.h>
#endif

namespace m2d {

std::ofstream Log::s_file;
bool          Log::s_initialized = false;
LogLevel      Log::s_minLevel    = LogLevel::Info;

void Log::Init(const std::string& filepath) {
    if (s_initialized) return;

    s_file.open(filepath, std::ios::out | std::ios::trunc);
    if (!s_file.is_open()) {
        std::cerr << "[Log] Failed to open log file: " << filepath << std::endl;
    }

    s_initialized = true;

    // Первая строка в логе — всегда отметка о старте
    WriteRaw(LogLevel::Info, "=== Log initialized ===");
}

void Log::Shutdown() {
    if (!s_initialized) return;

    WriteRaw(LogLevel::Info, "=== Log shutdown ===");

    if (s_file.is_open()) {
        s_file.close();
    }

    s_initialized = false;
}

void Log::WriteRaw(LogLevel level, const std::string& message) {
    if (level < s_minLevel) return;

    // ===== Время =====
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  now.time_since_epoch()) % 1000;

    std::tm tm_buf{};
#ifdef _WIN32
    localtime_s(&tm_buf, &time);
#else
    localtime_r(&time, &tm_buf);
#endif

    std::ostringstream ts;
    ts << std::put_time(&tm_buf, "%H:%M:%S")
       << "." << std::setfill('0') << std::setw(3) << ms.count();

    // ===== Формируем строку =====
    std::ostringstream line;
    line << "[" << ts.str() << "] "
         << "[" << LevelToString(level) << "] "
         << message;

    const std::string finalLine = line.str();

    // ===== 1. Файл =====
    if (s_file.is_open()) {
        s_file << finalLine << "\n";
        s_file.flush();   // чтобы при краше всё было видно
    }

    // ===== 2. Консоль =====
    std::ostream& out = (level == LogLevel::Error) ? std::cerr : std::cout;
    out << finalLine << std::endl;

    // ===== 3. OutputDebugString (Windows, для Visual Studio) =====
#ifdef _WIN32
    OutputDebugStringA((finalLine + "\n").c_str());
#endif
}

const char* Log::LevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::Info:  return "INFO";
        case LogLevel::Warn:  return "WARN";
        case LogLevel::Error: return "ERROR";
        case LogLevel::None:  return "NONE";
    }
    return "?";
}

} // namespace m2d