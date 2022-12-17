#ifndef THZ_COMMON_MATH_CONSTANTS_H
#define THZ_COMMON_MATH_CONSTANTS_H

#include <cstdint>

namespace Terrahertz {

/// <summary>
/// Pi.
/// </summary>
constexpr double Pi{3.141592653589793238462643383};

/// <summary>
/// Pi as a float.
/// </summary>
constexpr float PiF{3.141592653589793238462643383f};

/// @brief Directions for geometric operations.
enum class Direction : uint8_t
{
    /// @brief Y+
    Up = 0,

    /// @brief X+.
    Right = 1,

    /// @brief Y-.
    Down = 2,

    /// @brief X-.
    Left = 3
};

/// @brief Turns the given diretion clockwise.
///
/// @param d The direction to turn.
/// @return The turned direction.
constexpr Direction turnRight(Direction d) noexcept
{
    switch (d)
    {
    case Direction::Up:
        return Direction::Right;
    case Direction::Right:
        return Direction::Down;
    case Direction::Down:
        return Direction::Left;
    case Direction::Left:
    default:
        return Direction::Up;
    }
}

/// @brief Turns the given diretion counterclockwise.
///
/// @param d The direction to turn.
/// @return The turned direction.
constexpr Direction turnLeft(Direction d) noexcept
{
    switch (d)
    {
    case Direction::Up:
        return Direction::Left;
    case Direction::Right:
        return Direction::Up;
    case Direction::Down:
        return Direction::Right;
    case Direction::Left:
    default:
        return Direction::Down;
    }
}

} // namespace Terrahertz

#endif // !THZ_COMMON_MATH_CONSTANTS_H
