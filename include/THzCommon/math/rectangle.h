#ifndef THZ_COMMON_MATH_RECTANGLE_H
#define THZ_COMMON_MATH_RECTANGLE_H

#include "constants.h"
#include "point.h"

namespace Terrahertz {

/// @brief A rectangle with basic geometrical methods.
class Rectangle
{
public:
    /// @brief The upper left point of the rectangle.
    Point upperLeftPoint{};

    /// @brief The width of the rectangle.
    std::uint32_t width{0};

    /// @brief The height point of the rectangle.
    std::uint32_t height{0};

    /// @brief Default initializes a rectangle.
    Rectangle() noexcept = default;

    /// @brief Initializes a new rectangle.
    ///
    /// @param pWidth The width of the rectangle.
    /// @param pHeight The height of the rectangle.
    Rectangle(std::uint32_t const pWidth, std::uint32_t const pHeight) noexcept;

    /// @brief Initializes a new rectangle.
    ///
    /// @param pX X coordinate of the upper left point of the rectangle.
    /// @param pY Y coordinate of the upper left point of the rectangle.
    /// @param pWidth The width of the rectangle.
    /// @param pHeight The height of the rectangle.
    Rectangle(std::int32_t pX, std::int32_t pY, std::uint32_t const pWidth, std::uint32_t const pHeight) noexcept;

    /// @brief Initializes a new rectangle.
    ///
    /// @param pUpperLeftPoint The upper left point of the rectangle.
    /// @param pWidth The width of the rectangle.
    /// @param pHeight The height of the rectangle.
    Rectangle(Point const pUpperLeftPoint, std::uint32_t const pWidth, std::uint32_t const pHeight) noexcept;

    /// @brief Initializes a new rectangle using the area marked by two opposing corner points.
    ///
    /// @param a The first corner point of the rectangle.
    /// @param b The second corner point of the rectangle.
    /// @remarks The points should be on opposing corners of the rectangle.
    Rectangle(Point const a, Point const b) noexcept;

    /// @brief Explicitly default the copy constructor so all special methods are defined.
    Rectangle(Rectangle const &) noexcept = default;

    /// @brief Explicitly default the move constructor so all special methods are defined.
    Rectangle(Rectangle &&) noexcept = default;

    /// @brief Explicitly default the copy assignment operator so all special methods are defined.
    Rectangle &operator=(Rectangle const &) noexcept = default;

    /// Explicitly default the move assignment operator so all special methods are defined.
    Rectangle &operator=(Rectangle &&) noexcept = default;

    /// @brief Explicitly default the destructor to make it virtual.
    ~Rectangle() noexcept = default;

    /// @brief Calculates the area of the rectangle.
    ///
    /// @returns The area of this rectangle.
    uint32_t area() const noexcept;

    /// @brief Calculates the center point of the rectangle.
    ///
    /// @returns The center point of the rectangle.
    Point center() const noexcept;

    /// @brief Calculates the lower right point of the rectangle.
    ///
    /// @returns The lower right point of the rectangle.
    Point lowerRightPoint() const noexcept;

    /// @brief Calculates the intersection rectangle of this rectangle and another one.
    ///
    /// @param other The other rectangle to intersect with.
    /// @returns The rectangle representing the intersectional area.
    Rectangle intersection(Rectangle const &other) const noexcept;

    /// @brief Checks if the given rectangle is enclosed by this rectangle.
    ///
    /// @param other The other rectangle to check.
    /// @return True if this rectangle encloses the given one, false otherwise.
    bool encloses(Rectangle const &other) const noexcept;

    /// @brief Shifts one edge of this rectangle.
    ///
    /// @param direction The direction the edge is facing.
    /// @param value The amount of units the edge is shifted, positive is away from the center of the rectangle.
    void shiftEdge(Direction direction, std::int32_t value) noexcept;

    /// @brief Check if this rectangle equals an other rectangle.
    ///
    /// @param other The other rectangle.
    /// @returns True if the rectangles are equal, false otherwise.
    bool operator==(Rectangle const &other) const noexcept
    {
        return upperLeftPoint == other.upperLeftPoint && width == other.width && height == other.height;
    }

    /// @brief Check if this rectangle not equals an other rectangle.
    ///
    /// @param other The other rectangle.
    /// @returns True if the rectangles are not equal, false otherwise.
    bool operator!=(Rectangle const &other) const noexcept
    {
        return upperLeftPoint != other.upperLeftPoint || width != other.width || height != other.height;
    }
};

} // namespace Terrahertz

#endif // !THZ_COMMON_MATH_RECTANGLE_H
