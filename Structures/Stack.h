#ifndef THZ_COMMON_STRUCTURES_STACK_H
#define THZ_COMMON_STRUCTURES_STACK_H

#include <array>
#include <gsl/span>

namespace Terrahertz {

/// @brief Template implementation of a stack.
///
/// @tparam TValueType The type of values stored in the stack.
/// @tparam TBufferSize The size of the stack.
template <typename TValueType, size_t TBufferSize>
class Stack
{
public:
    /// @brief The value type of the stack.
    using value_type = TValueType;

    /// @brief Default initializes a new Stack instance.
    Stack() noexcept = default;

    /// @brief Pushes the given value onto the stack.
    ///
    /// @param value The value to push onto the stack.
    /// @remarks If the stack is full, the value will not be pushed.
    void push(TValueType const &value) noexcept
    {
        if (!full())
        {
            _buffer[_filled] = value;
            ++_filled;
        }
    }

    /// @brief Pops the top element from the stack.
    ///
    /// @return The top element.
    TValueType pop() noexcept
    {
        if (_filled > 0)
        {
            --_filled;
        }
        return _buffer[_filled];
    }

    /// @brief Pops elements of the stack until the given amount is left.
    ///
    /// @param element The amount of elements to be left in the buffer.
    /// @remaks Nothing changes if the stack already has less elements.
    void popTill(size_t element) noexcept
    {
        if (_filled > element)
        {
            _filled = element;
        }
    }

    /// @brief Clears the Stack.
    void clear() noexcept { _filled = 0U; }

    /// @brief Checks if the stack is full.
    ///
    /// @return True if the stack is full, false otherwise.
    inline bool full() const noexcept { return _filled == _buffer.size(); }

    /// @brief Returns the size of the currently filled part of the stack.
    ///
    /// @return The size of the currently filled part of the stack.
    inline size_t filled() const noexcept { return _filled; }

    /// @brief Returns the size of the stack.
    ///
    /// @return The size of the stack.
    constexpr size_t size() const noexcept { return TBufferSize; }

    /// @brief Returns a span of the currently filled part of the stack.
    ///
    /// @return A Span of the currently filled part of the stack.
    gsl::span<TValueType const> data() const noexcept
    {
        return gsl::span<TValueType const>{_buffer.data(), static_cast<ptrdiff_t>(filled())};
    }

    /// @brief Returns a span of the currently filled part of the stack.
    ///
    /// @return A Span of the currently filled part of the stack.
    gsl::span<TValueType> data() noexcept
    {
        return gsl::span<TValueType>{_buffer.data(), static_cast<ptrdiff_t>(filled())};
    }

    /// @brief Returns a pointer to the top element of the Stack.
    ///
    /// @return Pointer to the top element of the Stack.
    TValueType const *top() const noexcept { return _filled < 1U ? nullptr : &_buffer[_filled - 1]; }

    /// @brief Returns a pointer to the top element of the Stack.
    ///
    /// @return Pointer to the top element of the Stack.
    TValueType *top() noexcept { return _filled < 1U ? nullptr : &_buffer[_filled - 1]; }

private:
    /// @brief The amount filled space in the stack.
    size_t _filled{};

    /// @brief The buffer of the stack.
    std::array<TValueType, TBufferSize> _buffer{};
};

} // namespace Terrahertz

#endif // !THZ_COMMON_STRUCTURES_STACK_H
