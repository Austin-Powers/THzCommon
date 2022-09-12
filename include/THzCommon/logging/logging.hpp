#ifndef THZ_COMMON_LOGGING_LOGGING_HPP
#define THZ_COMMON_LOGGING_LOGGING_HPP

#include "Logging.h"

#include <array>

namespace Terrahertz {

template <LogLevel TLevel, Project TProject>
void Logger::log(std::string const &message, std::source_location const loc) noexcept
{
    log<TLevel, TProject>(message.data(), loc);
}

template <LogLevel TLevel, Project TProject>
void Logger::log(std::string_view const &message, std::source_location const loc) noexcept
{
    log<TLevel, TProject>(message.data(), loc);
}

template <LogLevel TLevel, Project TProject>
void Logger::log(gsl::czstring const message, std::source_location const loc) noexcept
{
    constexpr size_t fixedLineContentLength = sizeof "1970-01-01 00:00:00:000 E  ";

    if (TLevel <= _maxLevel)
    {
        auto const time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        struct tm  tmTime;
        gmtime_s(&tmTime, &time);
        auto const millis = static_cast<uint16_t>(time % 1000);

        std::array<char, fixedLineContentLength + ProjectNameLengthLimit> buffer{};
        snprintf(buffer.data(),
                 buffer.size(),
                 "%04u-%02u-%02u %02u:%02u:%02u:%03u %c %-s ",
                 tmTime.tm_year + 1900,
                 tmTime.tm_mon,
                 tmTime.tm_mday,
                 tmTime.tm_hour,
                 tmTime.tm_min,
                 tmTime.tm_sec,
                 millis,
                 getLogLevelCharacter<TLevel>(),
                 TProject::name());
        buffer[fixedLineContentLength - 2 + _maxProjectNameLength] = ' ';
        buffer[fixedLineContentLength - 1 + _maxProjectNameLength] = '\0';

        std::string text{buffer.data()};
        if (_logSourceLocation)
        {
            text += loc.file_name();
            text += '(';
            text += std::to_string(loc.line());
            text += ':';
            text += std::to_string(loc.column());
            text += ") ";
            text += loc.function_name();
            text += ": ";
        }
        text += message;
        logString(text);
    }
}
template <Project TProject>
void Logger::addProject() noexcept
{
    std::unique_lock lock{_loggerMutex};
    auto const       length = std::strlen(TProject::name());
    if (length > ProjectNameLengthLimit)
    {
        _maxProjectNameLength = ProjectNameLengthLimit;
        logMessage<LogLevel::Warning, LoggingProject>("Given project name exceeds ProjectNameLengthLimit");
    }
    else if (_maxProjectNameLength < length)
    {
        _maxProjectNameLength = static_cast<std::uint16_t>(length);
    }
}

template <LogLevel TLevel, Project TProject>
void logMessage(std::string const &message, std::source_location const loc) noexcept
{
    Logger::globalInstance().log<TLevel, TProject>(message, loc);
}

template <LogLevel TLevel, Project TProject>
void logMessage(std::string_view const &message, std::source_location const loc) noexcept
{
    Logger::globalInstance().log<TLevel, TProject>(message, loc);
}

template <LogLevel TLevel, Project TProject>
void logMessage(gsl::czstring const message, std::source_location const loc) noexcept
{
    Logger::globalInstance().log<TLevel, TProject>(message, loc);
}

template <Project TProject>
void addProjectName() noexcept
{
    Logger::globalInstance().addProject<TProject>();
}

} // namespace Terrahertz

#endif // !THZ_COMMON_LOGGING_LOGGING_HPP
