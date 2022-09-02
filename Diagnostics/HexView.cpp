#include "HexView.h"

#include <iostream>

namespace Terrahertz {
namespace Diagnostics {
namespace {

/// @brief Number of bytes displayed per line.
constexpr uint32_t BytesPerLine = 0x0010;

/// @brief The conversion table to convert bytes into characters.
char conversionTable[] = "................"
                         "................"
                         ".!\"#$%&\'()*+,-./"
                         "0123456789:;<=>?"
                         "@ABCDEFGHIJKLMNO"
                         "PQRSTUVWXYZ[\\]^_"
                         "`abcdefghijklmno"
                         "pqrstuvwxyz{|}~."
                         "................"
                         "................"
                         "................"
                         "................"
                         "................"
                         "................"
                         "................"
                         "................";

/// @brief The conversion table to convert numbers into hex digits.
char hexConversionTable[] = "0123456789ABCDEF";

/// @brief Initial state of the line buffer of each HexView.
char lineBufferInitialState[] = "LLLLLLLL  XX XX XX XX XX XX XX XX  XX XX XX XX XX XX XX XX  aaaaaaaaaaaaaaaa\r\n";

} // namespace

HexView::HexView() noexcept { std::memcpy(_lineBuffer, lineBufferInitialState, 79); }

void HexView::print(gsl::span<uint8_t const> data) noexcept
{
    gsl::span<char> lineSpan{_lineBuffer};
    gsl::span<char> offsetSpan = lineSpan.subspan(0, 8);
    gsl::span<char> textSpan   = lineSpan.subspan(60, BytesPerLine);

    for (auto offset = 0U; offset < data.size(); ++offset)
    {
        uint64_t const index = offset & 0xF;
        if (index == 0x0)
        {
            uint32_t number = offset;
            for (auto i = 7; i >= 0; --i)
            {
                offsetSpan[i] = hexConversionTable[number & 0xF];
                number >>= 4;
            }
        }
        auto const byte = data[offset];
        lineSpan[(index > 7 ? 11 : 10) + (index * 3)] = hexConversionTable[(byte >> 4) & 0xF];
        lineSpan[(index > 7 ? 12 : 11) + (index * 3)] = hexConversionTable[byte & 0xF];
        textSpan[index] = conversionTable[byte];

        if (index == 0xF)
        {
            std::cout << _lineBuffer;
        }
    }

    for (auto index = data.size() % BytesPerLine; index < BytesPerLine; ++index)
    {
        lineSpan[(index > 7 ? 11 : 10) + (index * 3)] = ' ';
        lineSpan[(index > 7 ? 12 : 11) + (index * 3)] = ' ';
        textSpan[index]                               = ' ';

        if (index == 0xF)
        {
            std::cout << _lineBuffer;
        }
    }

    std::cout.flush();
}

} // namespace Diagnostics
} // namespace Terrahertz
