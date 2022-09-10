#ifndef THZ_COMMON_LOGGING_LOGGING_H
#define THZ_COMMON_LOGGING_LOGGING_H

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
    void log(std::string const &message, std::source_location const loc = std::source_location::current()) noexcept;

    /// @brief Logs a message to console and log file.
    ///
    /// @tparam TLevel The level of the log message.
    /// @tparam TProject The proejct for which to log the message.
    /// @param message The message to log.
    /// @param loc The source_location the logMessage is called from.
    template <LogLevel TLevel, Project TProject>
    void log(std::string_view const    &message,
             std::source_location const loc = std::source_location::current()) noexcept;

    /// @brief Logs a message to console and log file.
    ///
    /// @tparam TLevel The level of the log message.
    /// @tparam TProject The proejct for which to log the message.
    /// @param message The message to log.
    /// @param loc The source_location the logMessage is called from.
    template <LogLevel TLevel, Project TProject>
    void log(gsl::czstring const message, std::source_location const loc = std::source_location::current()) noexcept;

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
    void addProject() noexcept;

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
void logMessage(std::string const &message, std::source_location const loc = std::source_location::current()) noexcept;

/// @brief Logs a message to console and log file.
///
/// @tparam TLevel The level of the log message.
/// @tparam TProject The proejct for which to log the message.
/// @param message The message to log.
/// @param loc The source_location the logMessage is called from.
template <LogLevel TLevel, Project TProject>
void logMessage(std::string_view const    &message,
                std::source_location const loc = std::source_location::current()) noexcept;

/// @brief Logs a message to console and log file.
///
/// @tparam TLevel The level of the log message.
/// @tparam TProject The proejct for which to log the message.
/// @param message The message to log.
/// @param loc The source_location the logMessage is called from.
template <LogLevel TLevel, Project TProject>
void logMessage(gsl::czstring const message, std::source_location const loc = std::source_location::current()) noexcept;

/// @brief Adds a project so the global logger can readjust the maxProjectNameLength.
///
/// @tparam TProject The project to add.
template <Project TProject>
void addProjectName() noexcept;

} // namespace Terrahertz

// include template implementation
#include "Logging.hpp"

#endif // !THZ_COMMON_LOGGING_LOGGING_H
