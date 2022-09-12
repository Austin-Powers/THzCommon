#include "THzCommon/diagnostics/stopwatch.h"

#include <iostream>

namespace Terrahertz {

void StopWatch::start() noexcept { _startTime = SystemNow; }

Milliseconds StopWatch::get() const noexcept
{
    auto const duration = SystemNow - _startTime;
    return std::chrono::duration_cast<Milliseconds>(duration);
}

void StopWatch::print() const noexcept { printf("%lld ms\n", get().count()); }

} // namespace Terrahertz
