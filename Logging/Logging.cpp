#include "Logging.h"

#include <chrono>
#include <iostream>

namespace Terrahertz {

template <>
char getLogLevelCharacter<LogLevel::Error>() noexcept
{
    return 'E';
}

template <>
char getLogLevelCharacter<LogLevel::Warning>() noexcept
{
    return 'W';
}

template <>
char getLogLevelCharacter<LogLevel::Info>() noexcept
{
    return 'I';
}

template <>
char getLogLevelCharacter<LogLevel::Trace>() noexcept
{
    return 'T';
}

Logger &Logger::globalInstance() noexcept
{
    static Logger instance{};
    return instance;
}

Logger::~Logger() noexcept { _logFile.close(); }

void Logger::setMaxLevel(LogLevel level) noexcept { _maxLevel = level; }

void Logger::setFilepath(std::string const &filepath) noexcept
{
    _filepath = filepath;
    if (!filepath.empty())
    {
        auto const time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        char       timeString[sizeof "20190820_0243.log"];
        struct tm  tmTime;
        gmtime_s(&tmTime, &time);
        strftime(timeString, sizeof timeString, "%Y%m%d_%H%M.log", &tmTime);
        _filepath += timeString;
    }
}

void Logger::logString(std::string const &text) noexcept
{
    std::unique_lock lock{_loggerMutex};
    std::cout << text << std::endl;

    if (!_filepath.empty() && !_logFile.is_open())
    {
        _logFile.open(_filepath, std::ofstream::out);
    }

    if (_logFile.is_open())
    {
        _logFile << text << std::endl;
    }
}

} // namespace Terrahertz
