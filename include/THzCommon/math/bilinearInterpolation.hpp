#ifndef THZ_COMMON_MATH_BILINEARINTERPOLATION_HPP
#define THZ_COMMON_MATH_BILINEARINTERPOLATION_HPP

#include "inrange.hpp"
#include "matrix.hpp"
#include "rectangle.hpp"

#include <array>
#include <gsl/gsl>
#include <optional>

namespace Terrahertz {

/// @brief Template class encapsulating the algorithm for bilinear interpolation.
///
/// @tparam TValueType The value type to perform the interpolation on.
template <typename TValueType>
class BilinearInterpolation
{
public:
    /// @brief Creates a BilinearInterpolation instance if the grid.size() of the dimensions.area().
    ///
    /// @param grid The grid of values to interpolate on.
    /// @param dimensions The dimensions of the grid.
    /// @return The created instance, if the values fit each other.
    static std::optional<BilinearInterpolation> create(gsl::span<TValueType const> const grid,
                                                       Rectangle const                  &dimensions) noexcept
    {
        if (dimensions.area() == grid.size())
        {
            return BilinearInterpolation{grid, dimensions};
        }
        return {};
    }

    /// @brief Default initializes a new BilinearInterpolation.
    BilinearInterpolation() noexcept = default;

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
    /// @brief Initializes a new BilinearInterpolation using the given grid of values.
    ///
    /// @param grid The grid of values to interpolate on.
    /// @param dimensions The dimensions of the grid.
    BilinearInterpolation(gsl::span<TValueType const> const grid, Rectangle const &dimensions) noexcept
        : _grid{grid}, _dimensions{dimensions}
    {
        // this will make sure the update is triggered on the first run
        _boundaries[0U] = 1.1;
    }

    /// @brief Updates the coefficients for the interpolation for the given coordinates.
    ///
    /// @param x The x-position for the coefficients.
    /// @param y The y-position for the coefficients.
    void updateCoefficients(double x, double y) noexcept
    {
        auto const lowerIndex = [](double const v, std::uint32_t const dimension) noexcept -> std::uint32_t {
            auto const dim = dimension - 1U;
            auto const v0  = static_cast<std::uint32_t>(v * dim);
            return (v0 == dim) ? v0 - 1U : v0;
        };
        auto const valueAt = [this](size_t const x, size_t const y) noexcept -> TValueType {
            return _grid[x + (_dimensions.width * y)];
        };

        // calculate new boundaries
        auto const x0Int = lowerIndex(x, _dimensions.width);
        auto const y0Int = lowerIndex(y, _dimensions.height);
        auto const x1Int = x0Int + 1U;
        auto const y1Int = y0Int + 1U;
        // we need to subtract one because we are addressing the areas between the points not the points themselves
        _boundaries[0U] = static_cast<double>(x0Int) / (_dimensions.width - 1U);
        _boundaries[1U] = static_cast<double>(x1Int) / (_dimensions.width - 1U);
        _boundaries[2U] = static_cast<double>(y0Int) / (_dimensions.height - 1U);
        _boundaries[3U] = static_cast<double>(y1Int) / (_dimensions.height - 1U);

        // calculate new coefficients
        auto const &x0 = _boundaries[0U];
        auto const &x1 = _boundaries[1U];
        auto const &y0 = _boundaries[2U];
        auto const &y1 = _boundaries[3U];

        Matrix<double, 4U, 4U> const transformationMatrix{
            x1 * y1, -x1 * y0, -x0 * y1, x0 * y0, -y1, y0, y1, -y0, -x1, x1, x0, -x0, 1.0, -1.0, -1.0, 1.0};
        Matrix<TValueType, 1U, 4U> const valueVector{
            valueAt(x0Int, y0Int), valueAt(x0Int, y1Int), valueAt(x1Int, y0Int), valueAt(x1Int, y1Int)};
        auto const scalingFactor = 1.0 / ((x1 - x0) * (y1 - y0));
        _coefficients            = (transformationMatrix * valueVector) * scalingFactor;
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
    std::array<TValueType, 4U> _coefficients{};
};

} // namespace Terrahertz

#endif // !THZ_COMMON_MATH_BILINEARINTERPOLATION_HPP
