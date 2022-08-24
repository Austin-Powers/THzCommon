#ifndef THZ_COMMON_LOGGING_LOGGING_H
#define THZ_COMMON_LOGGING_LOGGING_H

#include <concepts>
#include <cstdint>
#include <fstream>
#include <gsl/gsl>
#include <mutex>
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

/// @brief Logs messages to console and specified log file.
class Logger
{
public:
    /// @brief Returns the global logger instance.
    ///
    /// @returns The global logger intance.
    static Logger &globalInstance() noexcept;

    /// @brief Default initializes the logger.
    Logger() noexcept = default;

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

    /// Logs a message to console and log file.
    ///
    /// @tparam TLevel The level of the log message.
    /// @tparam TID The id of the project that created the message.
    /// @param message The message to log.
    template <LogLevel TLevel, Project TProject>
    void log(std::string const &message) noexcept;

    /// Logs a message to console and log file.
    ///
    /// @tparam TLevel The level of the log message.
    /// @tparam TID The id of the project that created the message.
    /// @param message The message to log.
    template <LogLevel TLevel, Project TProject>
    void log(gsl::czstring const message) noexcept;

    /// @brief Sets the maximum log level.
    ///
    /// @param level The maximum level of message logged.
    void setMaxLevel(LogLevel level) noexcept;

    /// @brief Sets the path of the log file.
    ///
    /// @param filepath The path of the log file.
    /// @remarks If the string is empty no log file will be created.
    /// At the end of the path timestamp and '.log' will be added.
    void setFilepath(std::string const &filepath) noexcept;

private:
    /// @brief Logs a text to console and file if present.
    ///
    /// @param text The text to log.
    void logString(std::string const &text) noexcept;

    /// @brief Mutex to lock the output.
    std::mutex _loggerMutex{};

    /// @brief Maximum level of log messages actually written to console and file.
    LogLevel _maxLevel{LogLevel::Error};

    /// @brief The path of the log file.
    std::string _filepath{};

    /// @brief The stream of the log file.
    std::ofstream _logFile{};
};

/// @brief Logs a message to console and log file.
///
/// @param level The level of the log message.
/// @param id The id of the project that created the message.
/// @param message The message to log.
template <LogLevel TLevel, Project TID>
void logMessage(std::string const &message) noexcept;

} // namespace Terrahertz

// include template implementation
#include "Logging.hpp"

#endif // !THZ_COMMON_LOGGING_LOGGING_H
