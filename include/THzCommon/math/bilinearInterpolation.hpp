#ifndef THZ_COMMON_MATH_BILINEARINTERPOLATION_H
#define THZ_COMMON_MATH_BILINEARINTERPOLATION_H

#include "inrange.h"
#include "rectangle.h"

#include <array>
#include <gsl/gsl>

namespace Terrahertz {

/// @brief Template class encapsulating the algorithm for bilinear interpolation.
///
/// @tparam TValueType The value type to perform the interpolation on.
template <typename TValueType>
class BilinearInterpolation
{
public:
    /// @brief Default initializes a new BilinearInterpolation.
    BilinearInterpolation() noexcept = default;

    /// @brief Initializes a new BilinearInterpolation using the given grid of values.
    ///
    /// @param grid The grid of values to interpolate on.
    /// @param dimensions The dimensions of the grid.
    BilinearInterpolation(gsl::span<TValueType const> const grid, Rectangle dimensions) noexcept {}

    /// @brief Interpolates the value at the given relative position.
    ///
    /// @param x The x position in the given grid (0.0 - 1.0).
    /// @param y The y position in the given grid (0.0 - 1.0).
    /// @param result The storage for the interpolation result.
    /// @return True if the position was within the bounds, false otherwise.
    bool interpolate(double x, double y, TValueType &result) noexcept
    {
        if (!(0.0 <= y && y <= 1.0 && 0.0 <= x && x <= 1.0))
        {
            return false;
        }
        if (x < _boundaries[0U] || _boundaries[1U] < x || y < _boundaries[2U] || _boundaries[3U] < y)
        {
            updateCoefficients(x, y);
        }
        result = _coefficients[0U] + (_coefficients[1U] * x) + (_coefficients[2U] * y) + (_coefficients[3U] * x * y);
        return true;
    }

private:
    /// @brief Updates the coefficients for the interpolation for the given coordinates.
    ///
    /// @param x The x-position for the coefficients.
    /// @param y The y-position for the coefficients.
    void updateCoefficients(double x, double y) noexcept
    {
        // calculate new boundaries
        auto const lowerIndex = [](double const v, std::uint32_t const dimension) noexcept -> std::uint32_t {
            auto const dim = dimension - 1U;
            auto const v0  = static_cast<std::uint32_t>(v * dim);
            return (v0 == dim) ? v0 - 1U : v0;
        };
        auto const x0Int = lowerIndex(x, _dimensions.width);
        auto const y0Int = lowerIndex(y, _dimensions.height);
        auto const x1Int = x0Int + 1U;
        auto const y1Int = y0Int + 1U;

        // calculate new coefficients
    }

    /// @brief The grid of values to interpolate on.
    gsl::span<TValueType const> _grid{};

    /// @brief The dimensions of the grid.
    Rectangle _dimensions{};

    /// @brief The boundaries in which the coefficients are valid.
    ///
    /// @remarks [x0, x1, y0, y1]
    std::array<double, 4U> _boundaries{};

    /// @brief The coefficients of the interpolation.
    ///
    /// @remarks [a00, a10, a01, a11]
    std::array<double, 4U> _coefficients{};
};

} // namespace Terrahertz

#endif // !THZ_COMMON_MATH_BILINEARINTERPOLATION_H
