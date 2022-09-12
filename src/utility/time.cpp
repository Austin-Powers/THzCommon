#include "THzCommon/utility/time.h"

namespace Terrahertz {

std::string currentTimestampString() noexcept
{
    auto time  = std::chrono::system_clock::now();
    auto timeT = std::chrono::system_clock::to_time_t(time);
    tm   timeL{};
    localtime_s(&timeL, &timeT);
    char timeStringBuffer[80];
    strftime(timeStringBuffer, 80, "%Y_%m_%d__%H_%M_%S", &timeL);
    return {timeStringBuffer};
}

} // namespace Terrahertz
