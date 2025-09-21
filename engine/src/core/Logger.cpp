#include <terrakit/core/Logger.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#else
#include <fstream>
#include <memory>
#include <chrono>
#include <iomanip>
#endif

namespace terrakit::core {

Logger& Logger::Instance() {
    static Logger instance;
    return instance;
}

void Logger::Debug(const std::string& message) {
    Log(LogLevel::DEBUG, message);
}

void Logger::Info(const std::string& message) {
    Log(LogLevel::INFO, message);
}

void Logger::Warning(const std::string& message) {
    Log(LogLevel::WARNING, message);
}

void Logger::Error(const std::string& message) {
    Log(LogLevel::ERROR, message);
}

void Logger::Log(LogLevel level, const std::string& message) {
    if (level < m_MinLevel) return;

#ifdef __EMSCRIPTEN__
    // WASM: Use Emscripten's console API
    std::string logLine = "[" + LevelToString(level) + "] " + message;

    if (level >= LogLevel::ERROR) {
        emscripten_console_error(logLine.c_str());
    } else if (level >= LogLevel::WARNING) {
        emscripten_console_warn(logLine.c_str());
    } else {
        emscripten_console_log(logLine.c_str());
    }
#else
    // Native: Use timestamped logging with optional file output
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::stringstream ss;
    ss << "[" << std::put_time(std::localtime(&time_t), "%H:%M:%S");
    ss << "." << std::setfill('0') << std::setw(3) << ms.count() << "] ";
    ss << "[" << LevelToString(level) + "] " << message;

    std::string logLine = ss.str();

    // Console output
    if (level >= LogLevel::ERROR) {
        std::cerr << logLine << std::endl;
    } else {
        std::cout << logLine << std::endl;
    }

    // File output if enabled
    if (m_LogFile && m_LogFile->is_open()) {
        *m_LogFile << logLine << std::endl;
        m_LogFile->flush();
    }
#endif
}

std::string Logger::LevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG:   return "DEBUG";
        case LogLevel::INFO:    return "INFO ";
        case LogLevel::WARNING: return "WARN ";
        case LogLevel::ERROR:   return "ERROR";
        default:                return "UNKNOWN";
    }
}

#ifndef __EMSCRIPTEN__
void Logger::EnableFileLogging(const std::string& filename) {
    m_LogFile = std::make_unique<std::ofstream>(filename, std::ios::app);
    if (!m_LogFile->is_open()) {
        std::cerr << "Failed to open log file: " << filename << std::endl;
        m_LogFile.reset();
    }
}

void Logger::DisableFileLogging() {
    if (m_LogFile) {
        m_LogFile->close();
        m_LogFile.reset();
    }
}
#endif

} // namespace terrakit::core