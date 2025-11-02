#ifndef THZ_COMMON_UTILITY_RANGE2DFOLDING_HPP
#define THZ_COMMON_UTILITY_RANGE2DFOLDING_HPP

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <utility>

namespace Terrahertz {

/// @brief Generator for folding the content of a buffer representing 2D data using 2 nested for-each loops.
class Range2DFolding
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

        /// @brief Sets up the values inside the position.
        ///
        /// @param newIndex The new index to put the values to.
        /// @param bufferWidth The width if the buffer in which the position is located.
        void setup(size_t const newIndex, std::uint32_t const bufferWidth) noexcept;

        /// @brief Returns the index value, so the position can be used in [].
        ///
        /// @return The index value.
        operator size_t() const noexcept;
    };

    /// @brief The inner iterator for the folding zone.
    class InnerIterator
    {
    public:
        /// @brief The value type of the iterator.
        using value_type = Position;

        /// @brief The difference type of the iterator.
        using difference_type = ptrdiff_t;

        /// @brief The size type of the iterator.
        using size_type = size_t;

        /// @brief Pointer to a value of this iterator.
        using pointer = value_type *;

        /// @brief Reference to a value of this iterator.
        using reference = value_type &;

        /// @brief Default initializes a new InnerIterator.
        InnerIterator() noexcept;

        /// @brief Initializes a new InnerIterator.
        ///
        /// @param bufferWidth The width of the buffer over which to iterate.
        /// @param zoneOffset The offset of the zone inside the buffer.
        /// @param zoneWidth The width of the zone inside the buffer.
        /// @param startIndex The index inside to buffer to start at.
        InnerIterator(std::uint32_t const bufferWidth,
                      std::uint32_t const zoneOffset,
                      std::uint32_t const zoneWidth,
                      size_t const        startIndex) noexcept;

        /// @brief Provides access to the index.
        ///
        /// @return The index of the iterator.
        [[nodiscard]] value_type operator*() const noexcept;

        /// @brief Increments the iterator.
        ///
        /// @return Reference to this iterator.
        InnerIterator &operator++() noexcept;

        /// @brief Increments the iterator.
        ///
        /// @return Copy of this iterator before being incremented.
        InnerIterator operator++(int) noexcept;

        /// @brief Compares this iterator to another iterator.
        ///
        /// @param other The other iterator to compare this one with.
        /// @return True if this iterators index equals the other ones, false otherwise.
        [[nodiscard]] bool operator==(InnerIterator const &other) const noexcept;

    private:
        /// @brief The width of the buffer over which to iterate.
        std::uint32_t _bufferWidth{};

        /// @brief The starting column of the iterator.
        std::uint32_t _xStart{};

        /// @brief The end column of the  iterator.
        std::uint32_t _xStop{};

        /// @brief The shift added to index once we enter the next line.
        std::uint32_t _shift{};

        /// @brief The current index of the iterator.
        Position _index{};
    };

    /// @brief The outer iterator for moving the folding zone.
    class OuterIterator
    {
    public:
        /// @brief The value type of the iterator.
        using value_type = OuterIterator;

        /// @brief The difference type of the iterator.
        using difference_type = ptrdiff_t;

        /// @brief The size type of the iterator.
        using size_type = size_t;

        /// @brief Pointer to a value of this iterator.
        using pointer = value_type *;

        /// @brief Reference to a value of this iterator.
        using reference = value_type &;

        /// @brief Default initializes a new OuterIterator.
        OuterIterator() noexcept;

        /// @brief Initializes a new OuterIterator.
        ///
        /// @param bufferWidth The width of the buffer.
        /// @param bufferHeight The height of the buffer.
        /// @param zoneWidth The width of the folding zone.
        /// @param zoneHeight The height of the folding zone.
        /// @param shiftX The cells the zone gets shifted on the X-axis.
        /// @param shiftY The cells the zone gets shifted on the Y-axis.
        /// @param startIndex The index inside to buffer to start at.
        OuterIterator(std::uint32_t const bufferWidth,
                      std::uint32_t const bufferHeight,
                      std::uint32_t const zoneWidth,
                      std::uint32_t const zoneHeight,
                      std::uint32_t const shiftX,
                      std::uint32_t const shiftY,
                      size_t const        startIndex = 0U) noexcept;

        /// @brief Provides access to the index.
        ///
        /// @return The index of the iterator.
        [[nodiscard]] value_type const &operator*() const noexcept;

        /// @brief Increments the iterator.
        ///
        /// @return Reference to this iterator.
        OuterIterator &operator++() noexcept;

        /// @brief Increments the iterator.
        ///
        /// @return Copy of this iterator before being incremented.
        OuterIterator operator++(int) noexcept;

        /// @brief Compares this iterator to another iterator.
        ///
        /// @param other The other iterator to compare this one with.
        /// @return True if this iterators index equals the other ones, false otherwise.
        [[nodiscard]] bool operator==(OuterIterator const &other) const noexcept;

        /// @brief Creates an InnerIterator with its index at the beginning of the buffer.
        ///
        /// @return An Iterator with its index at the beginning of the buffer.
        [[nodiscard]] InnerIterator begin() const noexcept;

        /// @brief Creates an InnerIterator with its index at the end of the buffer.
        ///
        /// @return An Iterator with its index at the end of the buffer.
        [[nodiscard]] InnerIterator end() const noexcept;

        /// @brief Returns the position in the target buffer.
        ///
        /// @return The position in the target buffer.
        [[nodiscard]] Position targetPosition() const noexcept;

    private:
        /// @brief The width of the buffer.
        std::uint32_t _bufferWidth{};

        /// @brief The height of the buffer.
        std::uint32_t _bufferHeight{};

        /// @brief The width of the folding zone.
        std::uint32_t _zoneWidth{};

        /// @brief The height of the folding zone.
        std::uint32_t _zoneHeight{};

        /// @brief The cells the zone gets shifted on the X-axis.
        std::uint32_t _shiftX{};

        /// @brief The cells the zone gets shifted on the Y-axis.
        std::uint32_t _shiftY{};

        /// @brief The current index of the iterator.
        Position _index{};

        /// @brief The index in the target buffer.
        Position _targetIndex{};
    };

    /// @brief Initialzes a new Range2DFolding instance.
    ///
    /// @param bufferWidth The width of the buffer.
    /// @param bufferHeight The height of the buffer.
    /// @param zoneWidth The width of the folding zone.
    /// @param zoneHeight The height of the folding zone.
    /// @param shiftX The cells the zone gets shifted on the X-axis.
    /// @param shiftY The cells the zone gets shifted on the Y-axis.
    Range2DFolding(std::uint32_t const bufferWidth,
                   std::uint32_t const bufferHeight,
                   std::uint32_t const zoneWidth,
                   std::uint32_t const zoneHeight,
                   std::uint32_t const shiftX,
                   std::uint32_t const shiftY) noexcept;

    /// @brief Creates an OuterIterator with its index at the beginning of the buffer.
    ///
    /// @return An Iterator with its index at the beginning of the buffer.
    [[nodiscard]] OuterIterator begin() const noexcept;

    /// @brief Creates an OuterIterator with its index at the end of the buffer.
    ///
    /// @return An Iterator with its index at the end of the buffer.
    [[nodiscard]] OuterIterator end() const noexcept;

    /// @brief Returns the width and height of the target buffer.
    ///
    /// @return Pair of width and height of the target buffer.
    [[nodiscard]] std::pair<std::uint32_t, std::uint32_t> targetDimensions() const noexcept;

private:
    /// @brief The width of the buffer.
    std::uint32_t _bufferWidth{};

    /// @brief The height of the buffer.
    std::uint32_t _bufferHeight{};

    /// @brief The width of the folding zone.
    std::uint32_t _zoneWidth{};

    /// @brief The height of the folding zone.
    std::uint32_t _zoneHeight{};

    /// @brief The cells the zone gets shifted on the X-axis.
    std::uint32_t _shiftX{};

    /// @brief The cells the zone gets shifted on the Y-axis.
    std::uint32_t _shiftY{};
};

} // namespace Terrahertz

#endif // !THZ_COMMON_UTILITY_RANGE2DFOLDING_HPP
