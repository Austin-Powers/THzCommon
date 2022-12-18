#ifndef THZ_COMMON_MATH_MATRIX_HPP
#define THZ_COMMON_MATH_MATRIX_HPP

#include <array>
#include <cstddef>

namespace Terrahertz {

/// @brief Represents a Matrix.
///
/// @tparam TValueType The value type of the matrix.
/// @tparam TColumns The number of columns of the matrix.
/// @tparam TRows The number of rows of the matrix.
template <typename TValueType, size_t TColumns, size_t TRows>
class Matrix
{
public:
    /// @brief Shortcut to the value type of this matrix.
    using value_type = TValueType;

    /// @brief Returns the number of columns of this matrix.
    ///
    /// @return The number of columns of this matrix.
    constexpr size_t columns() const noexcept { return TColumns; }

    /// @brief Returns the number of rows of this matrix.
    ///
    /// @return The number of rows of this matrix.
    constexpr size_t rows() const noexcept { return TRows; }

    /// @brief Default initialzes a new Matrix.
    Matrix() noexcept = default;

    /// @brief Returns the value at the given position of the matrix.
    ///
    /// @param x The column of the value.
    /// @param y The row of the value.
    /// @return The value.
    /// @remark This operator does not check if the position is valid.
    [[nodiscard]] TValueType operator()(size_t x, size_t y) const noexcept { return _data[x + (y * TColumns)]; }

    /// @brief Returns a reference to the value at the given position of the matrix.
    ///
    /// @param x The column of the value.
    /// @param y The row of the value.
    /// @return The value.
    /// @remark This operator does not check if the position is valid.
    [[nodiscard]] TValueType &operator()(size_t x, size_t y) noexcept { return _data[x + (y * TColumns)]; }

private:
    /// @brief The content of the matrix.
    std::array<TValueType, TColumns * TRows> _data{};
};

} // namespace Terrahertz

#endif // !THZ_COMMON_MATH_MATRIX_HPP
