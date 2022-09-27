#include "THzCommon/logging/logging.h"

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

Logger::Logger() noexcept { addProject<LoggingProject>(); }

Logger::~Logger() noexcept { _logFile.close(); }

LogLevel &Logger::maxLevel() noexcept { return _maxLevel; }

LogLevel Logger::maxLevel() const noexcept { return _maxLevel; }

std::string const &Logger::filepath() const noexcept { return _filepath; }

void Logger::setFilepath(std::string const &filepath) noexcept
{
    _filepath = filepath;
    if (!filepath.empty())
    {
        auto const time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        char       timeString[sizeof "20190820_0243.log"];
        struct tm  tmTime;
#if _WIN32
        gmtime_s(&tmTime, &time);
#else
        gmtime_r(&time, &tmTime);
#endif
        strftime(timeString, sizeof timeString, "%Y%m%d_%H%M.log", &tmTime);
        _filepath += timeString;
    }
}

std::uint16_t Logger::maxProjectNameLength() const noexcept { return _maxProjectNameLength; }

bool Logger::logSourceLocation() const noexcept { return _logSourceLocation; }

bool &Logger::logSourceLocation() noexcept { return _logSourceLocation; }

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
