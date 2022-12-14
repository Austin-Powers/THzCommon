#ifndef THZ_COMMON_MATH_MATRIX_HPP
#define THZ_COMMON_MATH_MATRIX_HPP

#include "THzCommon/logging/logging.hpp"
#include "THzCommon/utility/concepts.hpp"

#include <array>
#include <cstddef>
#include <initializer_list>
#include <type_traits>

namespace Terrahertz {

/// @brief Name provider for the THzCommon.Math.Matrix project.
struct MatrixProject
{
    static constexpr char const *name() noexcept { return "THzCommon.Math.Matrix"; }
};

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

    /// @brief Shortcut to the type of this matrix.
    using MyType = Matrix<TValueType, TColumns, TRows>;

    /// @brief Shortcut to the storage type of this matrix.
    using StorageType = std::array<TValueType, TColumns * TRows>;

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

    Matrix(std::initializer_list<TValueType> const &initializerList) noexcept
    {
        // as aggregate initialization is not applicable here we just copy by hand
        auto iter = _data.begin();
        for (auto const &entry : initializerList)
        {
            (*iter) = entry;
            ++iter;
            if (iter == _data.end())
            {
                logMessage<LogLevel::Warning, MatrixProject>(
                    "std::initializer_list has more entries than the matrix, cutting excess");
                break;
            }
        }
    }

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

    /// @brief Multiplies this matrix with the given scalar.
    ///
    /// @tparam TScalar The type of the scalar to multiply with.
    /// @param scalar The scalar to multiply with.
    /// @return Reference to this matrix.
    template <MultiplyableBy<TValueType> TScalar>
    MyType &operator*=(TScalar const &scalar) noexcept
    {
        for (auto &cell : _data)
        {
            cell *= scalar;
        }
        return *this;
    }

    /// @brief Multiplies this matrix with the given scalar and returns the result.
    ///
    /// @tparam TScalar The type of the scalar to multiply with.
    /// @param scalar The scalar to multiply with.
    /// @return The result of the multiplication.
    template <MultiplyableBy<TValueType> TScalar>
    [[nodiscard]] MyType operator*(TScalar const &scalar) const noexcept
    {
        auto copy = *this;
        copy *= scalar;
        return copy;
    }

    /// @brief Multiplies this matrix with the given matrix and returns the resulting matrix.
    ///
    /// @tparam OColumns The Columns of the other matrix.
    /// @param other The other matrix to multiply this matrix with.
    /// @return The resulting matrix.
    template <typename OValueType, size_t OColumns>
    auto operator*(Matrix<OValueType, OColumns, TColumns> const &other) const noexcept
        -> Matrix<MultiplicationResult_t<TValueType, OValueType>, OColumns, TRows>
    {
        Matrix<MultiplicationResult_t<TValueType, OValueType>, OColumns, TRows> result{};
        for (auto row = 0U; row < TRows; ++row)
        {
            for (auto column = 0U; column < OColumns; ++column)
            {
                auto &cell = result(column, row);
                for (auto index = 0U; index < TColumns; ++index)
                {
                    cell += operator()(index, row) * other(column, index);
                }
            }
        }
        return result;
    }

    /// @brief Conversion operator to std::array.
    ///
    /// @return The matrix converted to a std::array.
    operator StorageType() const noexcept { return _data; }

private:
    /// @brief The content of the matrix.
    StorageType _data{};
};

} // namespace Terrahertz

#endif // !THZ_COMMON_MATH_MATRIX_HPP
