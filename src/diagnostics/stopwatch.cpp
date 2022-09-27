#include "THzCommon/diagnostics/stopwatch.h"

#include <iostream>

namespace Terrahertz {

void StopWatch::start() noexcept { _startTime = SystemNow; }

Milliseconds StopWatch::get() const noexcept
{
    auto const duration = SystemNow - _startTime;
    return std::chrono::duration_cast<Milliseconds>(duration);
}

#if _WIN32
void StopWatch::print() const noexcept { printf("%lld ms\n", get().count()); }
#else
void StopWatch::print() const noexcept { printf("%ld ms\n", get().count()); }
#endif

} // namespace Terrahertz
