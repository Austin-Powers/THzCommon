#ifndef THZ_COMMON_LOGGING_LOGGING_HPP
#define THZ_COMMON_LOGGING_LOGGING_HPP

#include "Logging.h"

#include <array>

namespace Terrahertz {

template <LogLevel TLevel, Project TProject>
void Logger::log(std::string const &message) noexcept
{
    log<TLevel, TProject>(message.data());
}

template <LogLevel TLevel, Project TProject>
void Logger::log(gsl::czstring const message) noexcept
{
    if (TLevel <= _maxLevel)
    {
        auto const time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        struct tm  tmTime;
        gmtime_s(&tmTime, &time);
        auto const millis = static_cast<uint16_t>(time % 1000);

        std::array<char, 64> buffer{};
        snprintf(buffer.data(),
                 buffer.size(),
                 "%04u-%02u-%02u %02u:%02u:%02u:%03u %c %-21s ",
                 tmTime.tm_year + 1900,
                 tmTime.tm_mon,
                 tmTime.tm_mday,
                 tmTime.tm_hour,
                 tmTime.tm_min,
                 tmTime.tm_sec,
                 millis,
                 getLogLevelCharacter<TLevel>(),
                 TProject::name());

        std::string text{buffer.data()};
        text += message;
        logString(text);
    }
}

template <LogLevel TLevel, Project TProject>
void logMessage(std::string const &message) noexcept
{
    Logger::globalInstance().log<TLevel, TProject>(message);
}

} // namespace Terrahertz

#endif // !THZ_COMMON_LOGGING_LOGGING_HPP
