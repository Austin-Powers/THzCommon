#ifndef THZ_COMMON_MATH_POINT_HPP
#define THZ_COMMON_MATH_POINT_HPP

#include <cstdint>

namespace Terrahertz {

/// @brief A point in 2 dimensional space.
class Point
{
public:
    /// @brief The numerical type of the coordinates.
    using Coordinate = std::int32_t;

    /// @brief X coordinate
    Coordinate x{0};

    /// @brief Y coordinate
    Coordinate y{0};

    /// @brief Adds two points and returns the result.
    ///
    /// @param other The instance to add.
    /// @returns A new instance containing the result.
    [[nodiscard]] inline Point operator+(Point const &other) const noexcept
    {
        Point p;
        p.x = this->x + other.x;
        p.y = this->y + other.y;
        return p;
    }

    /// @brief Adds another point to this point.
    ///
    /// @param other The instance to add.
    /// @returns A reference to this instance.
    inline Point &operator+=(Point const &other) noexcept
    {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }

    /// @brief Subtracts two points and returns the result.
    ///
    /// @param other The instance to substract.
    /// @returns A new instance containing the result.
    [[nodiscard]] inline Point operator-(Point const &other) const noexcept
    {
        Point p;
        p.x = this->x - other.x;
        p.y = this->y - other.y;
        return p;
    }

    /// @brief Subtracts another point to this point.
    ///
    /// @param other The instance to substract.
    /// @returns A reference to this instance.
    inline Point &operator-=(Point const &other) noexcept
    {
        this->x -= other.x;
        this->y -= other.y;
        return *this;
    }

    /// @brief Multiplies this point with a scalar and returns the result.
    ///
    /// @param scalar The scalar to multiply with.
    /// @returns A new instance containing the result.
    [[nodiscard]] inline Point operator*(std::int32_t const scalar) const noexcept
    {
        Point p;
        p.x = this->x * scalar;
        p.y = this->y * scalar;
        return p;
    }

    /// @brief Multiplies this point with a scalar.
    ///
    /// @param scalar The scalar to multiply with.
    /// @returns A reference to this instance.
    inline Point operator*=(std::int32_t const scalar) noexcept
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    /// @brief Divides this point with by a value and returns the result.
    ///
    /// @param other The divisor.
    /// @returns A new instance containing the result.
    [[nodiscard]] inline Point operator/(std::int32_t const divisor) const noexcept
    {
        Point p;
        p.x = this->x / divisor;
        p.y = this->y / divisor;
        return p;
    }

    /// @brief Divides this point with by a value.
    ///
    /// @param other The divisor.
    /// @returns A reference to this instance.
    inline Point operator/=(std::int32_t const divisor) noexcept
    {
        x /= divisor;
        y /= divisor;
        return *this;
    }

    /// @brief Check if this point equals an other point.
    ///
    /// @param other The other point.
    /// @returns True if the points are equal, false otherwise.
    [[nodiscard]] inline bool operator==(Point const &other) const noexcept { return x == other.x && y == other.y; }

    /// @brief Check if this point not equals an other point.
    ///
    /// @param other The other point.
    /// @returns True if the points are not equal, false otherwise.
    [[nodiscard]] inline bool operator!=(Point const &other) const noexcept { return x != other.x || y != other.y; }

    /// @brief Calculates the distance from this Point to another Point.
    ///
    /// @param to The point the distance to is calculated.
    /// @returns The distance between this point and the other point.
    [[nodiscard]] double distance(Point const &to) const noexcept;

    /// @brief Calculates the angle between the x-axis and the line pointing to the other point.
    ///
    /// @param to The other point.
    /// @return The angle [rad].
    [[nodiscard]] double direction(Point const &to) const noexcept;

    /// @brief Shifts the point by the given distance in the given direction.
    ///
    /// @param direction The direction the shift is performed in [rad].
    /// @param distance The distance of the shift.
    /// @return The new point.
    [[nodiscard]] Point angularShift(double const direction, double const distance) const noexcept;
};

} // namespace Terrahertz

#endif // !THZ_COMMON_MATH_POINT_HPP
