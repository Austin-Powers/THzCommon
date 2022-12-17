#include "THzCommon/math/rectangle.hpp"

#include <algorithm>

namespace Terrahertz {

Rectangle::Rectangle(std::uint32_t const pWidth, std::uint32_t const pHeight) noexcept
    : upperLeftPoint{0, 0}, width{pWidth}, height{pHeight}
{}

Rectangle::Rectangle(std::int32_t const  pX,
                     std::int32_t const  pY,
                     std::uint32_t const pWidth,
                     std::uint32_t const pHeight) noexcept
    : upperLeftPoint{pX, pY}, width{pWidth}, height{pHeight}
{}

Rectangle::Rectangle(Point const pUpperLeftPoint, std::uint32_t const pWidth, std::uint32_t const pHeight) noexcept
    : upperLeftPoint{pUpperLeftPoint}, width{pWidth}, height{pHeight}
{}

Rectangle::Rectangle(Point const a, Point const b) noexcept
{
    upperLeftPoint.x = std::min(a.x, b.x);
    upperLeftPoint.y = std::min(a.y, b.y);

    width  = static_cast<std::uint32_t>(std::max(a.x, b.x) - upperLeftPoint.x);
    height = static_cast<std::uint32_t>(std::max(a.y, b.y) - upperLeftPoint.y);
}

uint32_t Rectangle::area() const noexcept { return width * height; }

Point Rectangle::center() const noexcept { return upperLeftPoint + Point(width / 2, height / 2); }

Point Rectangle::lowerRightPoint() const noexcept { return upperLeftPoint + Point(width, height); }

Rectangle Rectangle::intersection(Rectangle const &other) const noexcept
{
    // no intersection
    if (static_cast<std::int32_t>(upperLeftPoint.x + width) < other.upperLeftPoint.x ||
        static_cast<std::int32_t>(upperLeftPoint.y + height) < other.upperLeftPoint.y ||
        static_cast<std::int32_t>(other.upperLeftPoint.x + other.width) < upperLeftPoint.x ||
        static_cast<std::int32_t>(other.upperLeftPoint.y + other.height) < upperLeftPoint.y)
    {
        return {};
    }

    std::int32_t const x = std::max(upperLeftPoint.x, other.upperLeftPoint.x);
    std::int32_t const y = std::max(upperLeftPoint.y, other.upperLeftPoint.y);
    std::int32_t const w = std::min(upperLeftPoint.x + width, other.upperLeftPoint.x + other.width) - x;
    std::int32_t const h = std::min(upperLeftPoint.y + height, other.upperLeftPoint.y + other.height) - y;

    Rectangle intersectionRect{};
    intersectionRect.upperLeftPoint.x = x;
    intersectionRect.upperLeftPoint.y = y;
    intersectionRect.width            = static_cast<uint32_t>(std::max(0, w));
    intersectionRect.height           = static_cast<uint32_t>(std::max(0, h));
    return intersectionRect;
}

bool Rectangle::encloses(Rectangle const &other) const noexcept { return intersection(other) == other; }

void Rectangle::shiftEdge(Direction const direction, std::int32_t value) noexcept
{
    if (value < 0)
    {
        std::int32_t const maxShift = (direction == Direction::Up || direction == Direction::Down) ? height : width;

        value = std::max(value, -maxShift);
    }

    switch (direction)
    {
    case Direction::Up:
        upperLeftPoint.y += value;
        height += value;
        break;
    case Direction::Right:
        width += value;
        break;
    case Direction::Down:
        height += value;
        break;
    case Direction::Left:
        upperLeftPoint.x -= value;
        width += value;
        break;
    }
}

} // namespace Terrahertz
