#include "THzCommon/math/point.hpp"

#include <cmath>

namespace Terrahertz {

double Point::distance(Point const &to) const noexcept
{
    auto xDistance = to.x - x;
    auto yDistance = to.y - y;
    return std::sqrt(static_cast<double>((xDistance * xDistance) + (yDistance * yDistance)));
}

double Point::direction(Point const &to) const noexcept
{
    auto xDistance = to.x - x;
    auto yDistance = to.y - y;
    return std::atan2(yDistance, xDistance);
}

} // namespace Terrahertz
