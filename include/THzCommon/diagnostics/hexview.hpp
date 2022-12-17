#ifndef THZ_COMMON_ANALYTICS_HEXVIEW_HPP
#define THZ_COMMON_ANALYTICS_HEXVIEW_HPP

#include <cstdint>
#include <gsl/span>

namespace Terrahertz {
namespace Diagnostics {

/// @brief Viewer to conveniently display binary data on the console and export it human readable to files.
class HexView
{
public:
    /// @brief Initializes a new HexView instance.
    HexView() noexcept;

    /// @brief Prints the contents of the given span as a hex view to the console.
    ///
    /// @param data The span to print.
    void print(gsl::span<uint8_t const> data) noexcept;

private:
    /// @brief Buffer for the line.
    char _lineBuffer[79]{};
};

} // namespace Diagnostics
} // namespace Terrahertz

#endif // !THZ_COMMON_ANALYTICS_HEXVIEW_HPP
