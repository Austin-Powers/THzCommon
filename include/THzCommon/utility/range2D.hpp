#ifndef THZ_COMMON_UTILITY_RANGE2D_HPP
#define THZ_COMMON_UTILITY_RANGE2D_HPP

#include <cstddef>
#include <cstdint>
#include <iterator>

namespace Terrahertz {

/// @brief Generator for indexing a buffer representing 2D data using a for-each loop.
class Range2D
{
public:
    /// @brief The position in the buffer with x and y coordinates.
    struct Position
    {
        /// @brief The index in the buffer.
        size_t index{};

        /// @brief The X coordinate in the buffer.
        std::uint32_t x{};

        /// @brief The Y coordinate in the buffer.
        std::uint32_t y{};

        /// @brief Returns the index value, so the position can be used in [].
        ///
        /// @return The index value.
        operator size_t() const noexcept;
    };

    /// @brief The iterator created by the Range2D instance.
    class Iterator
    {
    public:
        /// @brief The value type of the iterator.
        using value_type = Position;

        /// @brief The difference type of this iterator.
        using difference_type = ptrdiff_t;

        /// @brief The size type of this iterator.
        using size_type = size_t;

        /// @brief Pointer to a value of this iterator.
        using pointer = value_type *;

        /// @brief Reference to a value of this iterator.
        using reference = value_type &;

        /// @brief Default initializes a new Iterator.
        Iterator() noexcept = default;

        /// @brief Initializes a new Iterator.
        ///
        /// @param width The width of the buffer.
        /// @param startIndex The index in the array at which to start.
        Iterator(std::uint32_t const width, size_t const startIndex = 0U) noexcept;

        /// @brief Provides access to the index.
        ///
        /// @return The index of the iterator.
        [[nodiscard]] value_type operator*() const noexcept;

        /// @brief Increments the iterator.
        ///
        /// @return Reference to this iterator.
        Iterator &operator++() noexcept;

        /// @brief Increments the iterator.
        ///
        /// @return Copy of this iterator before being incremented.
        Iterator operator++(int) noexcept;

        /// @brief Compares this iterator to another iterator.
        ///
        /// @param other The other iterator to compare this one with.
        /// @return True if this iterators index equals the other ones, false otherwise.
        [[nodiscard]] bool operator==(Iterator const &other) const noexcept;

    private:
        /// @brief The width of the buffer.
        std::uint32_t _width{};

        /// @brief The current index of the iterator.
        Position _index{};
    };

    /// @brief Initializes a new Range2D instance.
    ///
    /// @param width The width of the buffer.
    /// @param height The height of the buffer.
    Range2D(std::uint32_t width, std::uint32_t height) noexcept;

    /// @brief Creates an Iterator with its index at the beginning of the buffer.
    ///
    /// @return An Iterator with its index at the beginning of the buffer.
    [[nodiscard]] Iterator begin() const noexcept;

    /// @brief Creates an Iterator with its index at the end of the buffer.
    ///
    /// @return An Iterator with its index at the end of the buffer.
    [[nodiscard]] Iterator end() const noexcept;

private:
    /// @brief The width of the buffer.
    std::uint32_t _width{};

    /// @brief The height of the buffer.
    std::uint32_t _height{};
};

} // namespace Terrahertz

#endif // !THZ_COMMON_UTILITY_RANGE2D_HPP
