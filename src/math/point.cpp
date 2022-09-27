#include "THzCommon/math/point.h"

#include <cmath>

namespace Terrahertz {

double Point::distance(Point const &to) const noexcept
{
    auto xDistance = x - to.x;
    auto yDistance = y - to.y;
    return std::sqrt(static_cast<double>((xDistance * xDistance) + (yDistance * yDistance)));
}

} // namespace Terrahertz
