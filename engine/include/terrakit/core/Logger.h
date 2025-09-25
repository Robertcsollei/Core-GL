#pragma once

#include <iostream>
#include <string>
#include <sstream>

#ifndef TERRAKIT_EMSCRIPTEN
#include <memory>
#include <fstream>
#endif

namespace terrakit::core {

enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3
};

class Logger {
public:
    static Logger& Instance();

    void SetLevel(LogLevel level) { m_MinLevel = level; }

    void Debug(const std::string& message);
    void Info(const std::string& message);
    void Warning(const std::string& message);
    void Error(const std::string& message);

#ifndef TERRAKIT_EMSCRIPTEN
    void EnableFileLogging(const std::string& filename);
    void DisableFileLogging();
#endif

private:
    Logger() = default;
    void Log(LogLevel level, const std::string& message);
    std::string LevelToString(LogLevel level);

    LogLevel m_MinLevel = LogLevel::INFO;

#ifndef TERRAKIT_EMSCRIPTEN
    std::unique_ptr<std::ofstream> m_LogFile;
#endif
};

#define TK_DEBUG(msg) terrakit::core::Logger::Instance().Debug(msg)
#define TK_INFO(msg) terrakit::core::Logger::Instance().Info(msg)
#define TK_WARNING(msg) terrakit::core::Logger::Instance().Warning(msg)
#define TK_ERROR(msg) terrakit::core::Logger::Instance().Error(msg)

} // namespace terrakit::core