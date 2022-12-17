#ifndef THZ_COMMON_ANALYTICS_STOPWATCH_H
#define THZ_COMMON_ANALYTICS_STOPWATCH_H

#include "THzCommon/utility/time.h"

#include <chrono>
#include <cstdint>

namespace Terrahertz {

/// @brief Class for measuring the duration of certain processes and operations.
class StopWatch
{
public:
    // @brief Default initializes a new StopWatch instance.
    StopWatch() noexcept = default;

    /// @brief Starts the StopWatch.
    void start() noexcept;

    /// @brief Returns the milliseconds since the last call of start().
    ///
    /// @return The milliseconds since the last call of start().
    Milliseconds get() const noexcept;

    /// @brief Prints the milliseconds since the last call of start on the console.
    void print() const noexcept;

private:
    SystemTimePoint _startTime{};
};

} // namespace Terrahertz

#endif // !THZ_COMMON_ANALYTICS_STOPWATCH_H
