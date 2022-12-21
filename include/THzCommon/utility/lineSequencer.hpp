#ifndef THZ_COMMON_UTILITY_LINESEQUENCER_H
#define THZ_COMMON_UTILITY_LINESEQUENCER_H

#include <cstdint>
#include <gsl/gsl>
#include <optional>

namespace Terrahertz {

/// @brief Class for accessing a buffer line by line.
///
/// @tparam TValueType The type of the buffer.
template <typename TValueType>
class LineSequencer
{
public:
    /// @brief Creates a new LineSequencer, if the buffer.size() is a multiple of width.
    ///
    /// @param buffer The buffer to split into lines.
    /// @param width The width of each line.
    /// @param back Flag signalling if the line should be sequenced starting from the front or back of the buffer.
    /// @return The sequencer, if buffer.size() is a multiple of width.
    [[nodiscard]] static std::optional<LineSequencer>
    create(gsl::span<TValueType> buffer, std::uint32_t width, bool back = true) noexcept
    {
        if ((width != 0U) && ((buffer.size() % width) == 0U))
        {
            return LineSequencer{buffer, width, back};
        }
        return {};
    }

    /// @brief Default initializes a new LineSequencer.
    LineSequencer() noexcept = default;

    /// @brief Returns the next line in the buffer.
    ///
    /// @return The next line, or empty span if buffer as fully traversed.
    [[nodiscard]] gsl::span<TValueType> nextLine() noexcept
    {
        gsl::span<TValueType> result{};
        if (!_buffer.empty())
        {
            if (_back)
            {
                result  = _buffer.subspan(_buffer.size() - _width, _width);
                _buffer = _buffer.subspan(0U, _buffer.size() - _width);
            }
            else
            {
                result  = _buffer.subspan(0U, _width);
                _buffer = _buffer.subspan(_width);
            }
        }
        return result;
    }

private:
    /// @brief Initializes a new LineSequencer using the given parameters.
    ///
    /// @param buffer The buffer to split into lines.
    /// @param width The width of each line.
    /// @param bottomUp Flag signalling if the line should be sequenced starting from the front or back of the buffer.
    LineSequencer(gsl::span<TValueType> buffer, std::uint32_t width, bool back) noexcept
        : _buffer{buffer}, _width{width}, _back{back}
    {}

    /// @brief The remaining buffer.
    gsl::span<TValueType> _buffer{};

    /// @brief The line width.
    std::uint32_t _width{};

    /// @brief Flag signalling if the line should be sequenced starting from the front or back of the buffer.
    bool _back{};
};

} // namespace Terrahertz

#endif // !THZ_COMMON_UTILITY_LINESEQUENCER_H
