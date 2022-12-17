#ifndef THZ_COMMON_LOGGING_LOGGING_HPP
#define THZ_COMMON_LOGGING_LOGGING_HPP

#include <array>
#include <concepts>
#include <cstdint>
#include <fstream>
#include <gsl/gsl>
#include <mutex>
#include <source_location>
#include <string>
#include <string_view>

namespace Terrahertz {

/// @brief Enumeration of the different log levels.
enum class LogLevel : std::uint8_t
{
    /// @brief The log level for errors.
    Error = 0,

    /// @brief The log level for warnings.
    Warning = 1,

    /// @brief The log level for information.
    Info = 2,

    /// @brief The trace log level.
    Trace = 3
};

/// @brief Resolve LogLevel to the log level character.
///
/// @returns Character marking the log level.
template <LogLevel TLevel>
char getLogLevelCharacter() noexcept = delete;

template <>
char getLogLevelCharacter<LogLevel::Error>() noexcept;

template <>
char getLogLevelCharacter<LogLevel::Warning>() noexcept;

template <>
char getLogLevelCharacter<LogLevel::Info>() noexcept;

template <>
char getLogLevelCharacter<LogLevel::Trace>() noexcept;

// clang-format off
/// @brief Concept for ProjectClass, able to provide a name to the Logger.
template <typename TProjectClass>
concept Project = requires
{
    {TProjectClass::name()} -> std::same_as<char const *>;
};
// clang-format on

/// @brief Name provider for the logging project.
struct LoggingProject
{
    static constexpr char const *name() noexcept { return "THzCommon.Logging"; }
};

/// @brief Logs messages to console and specified log file.
class Logger
{
public:
    /// @brief The limit for the maxProjectNameLength.
    static constexpr std::uint16_t ProjectNameLengthLimit{48U};

    /// @brief Returns the global logger instance.
    ///
    /// @returns The global logger intance.
    static Logger &globalInstance() noexcept;

    /// @brief Default initializes the logger.
    Logger() noexcept;

    /// @brief Prevent copy construction by explicitly deleting the constructor.
    Logger(Logger const &) = delete;

    /// @brief Prevent move construction by explicitly deleting the constructor.
    Logger(Logger &&) = delete;

    /// @brief Prevent copy assignment by explicitly deleting the operator.
    Logger &operator=(Logger const &) = delete;

    /// @brief Prevent move assignment by explicitly deleting the operator.
    Logger &operator=(Logger &&) = delete;

    /// @brief Finalizes the logger instance.
    ~Logger() noexcept;

    /// @brief Logs a message to console and log file.
    ///
    /// @tparam TLevel The level of the log message.
    /// @tparam TProject The proejct for which to log the message.
    /// @param message The message to log.
    /// @param loc The source_location the logMessage is called from.
    template <LogLevel TLevel, Project TProject>
    void log(std::string const &message, std::source_location const loc = std::source_location::current()) noexcept
    {
        log<TLevel, TProject>(message.data(), loc);
    }

    /// @brief Logs a message to console and log file.
    ///
    /// @tparam TLevel The level of the log message.
    /// @tparam TProject The proejct for which to log the message.
    /// @param message The message to log.
    /// @param loc The source_location the logMessage is called from.
    template <LogLevel TLevel, Project TProject>
    void log(std::string_view const &message, std::source_location const loc = std::source_location::current()) noexcept
    {
        // string_view may not be zero terminated
        std::array<char, 128U> buffer{};
        std::memcpy(buffer.data(), message.data(), std::min(buffer.size(), message.size());
        log<TLevel, TProject>(buffer.data(), loc);
    }

    /// @brief Logs a message to console and log file.
    ///
    /// @tparam TLevel The level of the log message.
    /// @tparam TProject The proejct for which to log the message.
    /// @param message The message to log.
    /// @param loc The source_location the logMessage is called from.
    template <LogLevel TLevel, Project TProject>
    void log(gsl::czstring const message, std::source_location const loc = std::source_location::current()) noexcept
    {
        constexpr size_t fixedLineContentLength = sizeof "1970-01-01 00:00:00:000 E  ";

        if (TLevel <= _maxLevel)
        {
            auto const time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            struct tm  tmTime;
#if _WIN32
            localtime_s(&tmTime, &time);
#else
            localtime_r(&time, &tmTime);
#endif
            auto const millis = static_cast<uint16_t>(time % 1000);

            std::array<char, fixedLineContentLength + ProjectNameLengthLimit> buffer{};
            snprintf(buffer.data(),
                     buffer.size(),
                     "%04u-%02u-%02u %02u:%02u:%02u:%03u %c %-s ",
                     tmTime.tm_year + 1900,
                     tmTime.tm_mon + 1,
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

    /// @brief Provides access to the maximum LogLevel.
    ///
    /// @return A reference to the maximum LogLevel.
    LogLevel &maxLevel() noexcept;

    /// @brief Returns the maximum LogLevel.
    ///
    /// @return The maximum LogLevel.
    LogLevel maxLevel() const noexcept;

    /// @brief Provides acces the path of the log file.
    ///
    /// @return A reference to the path of the log file.
    std::string const &filepath() const noexcept;

    /// @brief Sets a new path for the log file of the logger.
    ///
    /// @param path The new path for the log file.
    void setFilepath(std::string const &path) noexcept;

    /// @brief Returns the maximum length of the known project names.
    ///
    /// @return The maximum length of the known project names.
    std::uint16_t maxProjectNameLength() const noexcept;

    /// @brief Adds a project so the logger can readjust the maxProjectNameLength.
    ///
    /// @tparam TProject The project to add.
    template <Project TProject>
    void addProject() noexcept
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

    /// @brief Returns the flag signalling if the source_location should be logged as well.
    ///
    /// @return The flag signalling if the source_location should be logged as well.
    bool logSourceLocation() const noexcept;

    /// @brief Returns a reference to the flag signalling if the source_location should be logged as well.
    ///
    /// @return A reference to the flag signalling if the source_location should be logged as well.
    bool &logSourceLocation() noexcept;

private:
    /// @brief Logs a text to console and file if present.
    ///
    /// @param text The text to log.
    void logString(std::string const &text) noexcept;

    /// @brief Mutex to lock the output.
    std::mutex _loggerMutex{};

    /// @brief Maximum level of log messages actually written to console and file.
    LogLevel _maxLevel{LogLevel::Error};

    /// @brief Flag signalling if the source_location should be logged as well.
    bool _logSourceLocation{};

    /// @brief The maximum length of the project names.
    std::uint16_t _maxProjectNameLength{};

    /// @brief The path of the log file.
    std::string _filepath{};

    /// @brief The stream of the log file.
    std::ofstream _logFile{};
};

/// @brief Logs a message to console and log file.
///
/// @tparam TLevel The level of the log message.
/// @tparam TProject The proejct for which to log the message.
/// @param message The message to log.
/// @param loc The source_location the logMessage is called from.
template <LogLevel TLevel, Project TProject>
void logMessage(std::string const &message, std::source_location const loc = std::source_location::current()) noexcept
{
    Logger::globalInstance().log<TLevel, TProject>(message, loc);
}

/// @brief Logs a message to console and log file.
///
/// @tparam TLevel The level of the log message.
/// @tparam TProject The proejct for which to log the message.
/// @param message The message to log.
/// @param loc The source_location the logMessage is called from.
template <LogLevel TLevel, Project TProject>
void logMessage(std::string_view const    &message,
                std::source_location const loc = std::source_location::current()) noexcept
{
    Logger::globalInstance().log<TLevel, TProject>(message, loc);
}

/// @brief Logs a message to console and log file.
///
/// @tparam TLevel The level of the log message.
/// @tparam TProject The proejct for which to log the message.
/// @param message The message to log.
/// @param loc The source_location the logMessage is called from.
template <LogLevel TLevel, Project TProject>
void logMessage(gsl::czstring const message, std::source_location const loc = std::source_location::current()) noexcept
{
    Logger::globalInstance().log<TLevel, TProject>(message, loc);
}

/// @brief Adds a project so the global logger can readjust the maxProjectNameLength.
///
/// @tparam TProject The project to add.
template <Project TProject>
void addProjectName() noexcept
{
    Logger::globalInstance().addProject<TProject>();
}

} // namespace Terrahertz

#endif // !THZ_COMMON_LOGGING_LOGGING_HPP
