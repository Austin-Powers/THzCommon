#ifndef THZ_COMMON_STRUCTURES_QUEUE_H
#define THZ_COMMON_STRUCTURES_QUEUE_H

#include <array>
#include <gsl/span>
#include <iostream>

namespace Terrahertz {

/// @brief Template implementation of a queue.
///
/// @tparam TValueType The type of values stored in the queue.
/// @tparam TBufferSize The size of the queue.
template <typename TValueType, size_t TBufferSize>
class Queue
{
public:
    /// @brief The value type of the queue.
    using value_type = TValueType;

    /// @brief Default initializes a new Queue instance.
    Queue() noexcept = default;

    /// @brief Pushes a new value to the back of the queue.
    ///
    /// @param value The value to push into the queue.
    void push(TValueType const &value) noexcept
    {
        if (_pushPosition == TBufferSize)
        {
            size_t i = 0U;
            size_t j = _popPosition;
            while (j != _pushPosition)
            {
                _buffer[i] = _buffer[j];
                ++i;
                ++j;
            }
            _pushPosition -= _popPosition;
            _popPosition = 0U;
        }

        if (_pushPosition < _buffer.size())
        {
            _buffer[_pushPosition] = value;
        }
        ++_pushPosition;
    }

    /// @brief Pops a certain amount of values at the front of the queue.
    ///
    /// @param amount The amount of values to pop.
    /// @remarks If more values are popped than are in the queue, it will be reset.
    void pop(size_t amount) noexcept
    {
        _popPosition += amount;
        if (_popPosition > _pushPosition)
        {
            _popPosition  = 0U;
            _pushPosition = 0U;
        }
    }

    /// @brief Checks if the queue is empty.
    ///
    /// @return True if the queue is empty, false otherwise.
    inline bool empty() const noexcept { return _popPosition == _pushPosition; }

    /// @brief Returns the size of the currently filled part of the queue.
    ///
    /// @return The size of the currently filled part of the queue.
    size_t filled() const noexcept { return _pushPosition - _popPosition; }

    /// @brief Returns the size of the queue.
    ///
    /// @return The size of the queue.
    constexpr size_t size() const noexcept { return TBufferSize; }

    /// @brief Returns a span of the currently filled part of the queue.
    ///
    /// @return A Span of the currently filled part of the queue.
    gsl::span<TValueType const> data() const noexcept
    {
        return gsl::span<TValueType const>{_buffer.data() + _popPosition, static_cast<ptrdiff_t>(filled())};
    }

    /// @brief Returns a span of the currently filled part of the queue.
    ///
    /// @return A Span of the currently filled part of the queue.
    gsl::span<TValueType> data() noexcept
    {
        return gsl::span<TValueType>{_buffer.data() + _popPosition, static_cast<ptrdiff_t>(filled())};
    }

private:
    /// @brief The position in the buffer to pop the next value from.
    size_t _popPosition{};

    /// @brief The position in the buffer to push the next value to.
    size_t _pushPosition{};

    /// @brief The buffer of the queue.
    std::array<TValueType, TBufferSize> _buffer{};
};

} // namespace Terrahertz

#endif // !THZ_COMMON_STRUCTURES_QUEUE_H
