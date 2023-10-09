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

Point Point::angularShift(double const direction, double const distance) const noexcept
{
    return {x + static_cast<Coordinate>(std::round(std::cos(direction) * distance)),
            y + static_cast<Coordinate>(std::round(std::sin(direction) * distance))};
}

} // namespace Terrahertz
