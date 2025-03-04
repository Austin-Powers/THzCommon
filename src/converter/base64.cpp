#include "THzCommon/converter/base64.hpp"

#include "THzCommon/logging/logging.hpp"
#include "THzCommon/utility/spanhelpers.hpp"

namespace Terrahertz::Base64 {

/// @brief Name provider for the base64 project.
struct Base64Project
{
    static constexpr char const *name() noexcept { return "THzCommon.Converter.Base64"; }
};

/// @brief Maps 6 bit sequences to characters.
constexpr char encodingTable[64U]{'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                  'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                  'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

/// @brief Maps characters to 6 bit sequences or 0xFF if the character is not used to encode data or 0x00 for '='.
constexpr std::uint8_t decodingTable[256U]{
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0x3EU, 0xFFU, 0xFFU, 0xFFU, 0x3FU,
    0x34U, 0x35U, 0x36U, 0x37U, 0x38U, 0x39U, 0x3AU, 0x3BU, 0x3CU, 0x3DU, 0xFFU, 0xFFU, 0xFFU, 0x00U, 0xFFU, 0xFFU,
    0xFFU, 0x00U, 0x01U, 0x02U, 0x03U, 0x04U, 0x05U, 0x06U, 0x07U, 0x08U, 0x09U, 0x0AU, 0x0BU, 0x0CU, 0x0DU, 0x0EU,
    0x0FU, 0x10U, 0x11U, 0x12U, 0x13U, 0x14U, 0x15U, 0x16U, 0x17U, 0x18U, 0x19U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xFFU, 0x1AU, 0x1BU, 0x1CU, 0x1DU, 0x1EU, 0x1FU, 0x20U, 0x21U, 0x22U, 0x23U, 0x24U, 0x25U, 0x26U, 0x27U, 0x28U,
    0x29U, 0x2AU, 0x2BU, 0x2CU, 0x2DU, 0x2EU, 0x2FU, 0x30U, 0x31U, 0x32U, 0x33U, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU,
    0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU, 0xFFU};

size_t encodedSize(size_t const byteCount) noexcept { return ((byteCount + 2) / 3) * 4; }

size_t decodedSize(size_t const symbolCount) noexcept { return (symbolCount / 4) * 3; }

gsl::span<char> encode(gsl::span<std::uint8_t const> const input, gsl::span<char> output) noexcept
{
    if (output.size() < encodedSize(input.size()))
    {
        logMessage<LogLevel::Error, Base64Project>("buffer given for encoded data is too small");
        return {};
    }

    size_t i{};
    auto   buffer = output;
    for (; input.size() - i >= 3U; i += 3U)
    {
        auto const index0 = (input[i] >> 2U);
        auto const index1 = ((input[i] << 4U) | (input[i + 1U] >> 4U)) & 0x3FU;
        auto const index2 = ((input[i + 1U] << 2U) | (input[i + 2U] >> 6U)) & 0x3FU;
        auto const index3 = input[i + 2U] & 0x3FU;

        buffer = writeToSpan(buffer, encodingTable[index0]);
        buffer = writeToSpan(buffer, encodingTable[index1]);
        buffer = writeToSpan(buffer, encodingTable[index2]);
        buffer = writeToSpan(buffer, encodingTable[index3]);
    }
    switch (input.size() - i)
    {
    case 1: {
        auto const index0 = (input[i + 0U] >> 2U);
        auto const index1 = (input[i + 0U] << 4U) & 0x3FU;

        buffer = writeToSpan(buffer, encodingTable[index0]);
        buffer = writeToSpan(buffer, encodingTable[index1]);
        buffer = writeToSpan(buffer, '=');
        buffer = writeToSpan(buffer, '=');
        break;
    }
    case 2: {
        auto const index0 = (input[i + 0U] >> 2U);
        auto const index1 = ((input[i + 0U] << 4U) | (input[i + 1U] >> 4U)) & 0x3FU;
        auto const index2 = (input[i + 1U] << 2U) & 0x3FU;

        buffer = writeToSpan(buffer, encodingTable[index0]);
        buffer = writeToSpan(buffer, encodingTable[index1]);
        buffer = writeToSpan(buffer, encodingTable[index2]);
        buffer = writeToSpan(buffer, '=');
        break;
    }
    }
    return output.subspan(0U, output.size() - buffer.size());
}

gsl::span<std::uint8_t> decode(gsl::span<char const> const input, gsl::span<std::uint8_t> output) noexcept
{
    if (input.empty())
    {
        return {};
    }
    if ((input.size() & 0b11U) != 0U)
    {
        logMessage<LogLevel::Error, Base64Project>("input length must be a multiple of four");
        return {};
    }
    if (output.size() < decodedSize(input.size()))
    {
        logMessage<LogLevel::Error, Base64Project>("buffer given for decoded data is too small");
        return {};
    }

    auto const equalSignsEnd = [&input]() noexcept -> std::uint8_t {
        if (input[input.size() - 1U] == '=')
        {
            if (input[input.size() - 2U] == '=')
            {
                return 2U;
            }
            return 1U;
        }
        return 0U;
    }();

    std::uint8_t equalSignsTotal{};
    for (auto const symbol : input)
    {
        if (symbol == '=')
        {
            ++equalSignsTotal;
            if (equalSignsTotal > equalSignsEnd)
            {
                logMessage<LogLevel::Error, Base64Project>("input string had equal sign in the wrong place");
                return {};
            }
        }
        if (decodingTable[symbol] == 0xFFU)
        {
            logMessage<LogLevel::Error, Base64Project>("Illegal character found in base64 encoded string");
            return {};
        }
    }

    auto remainingSymbols = input;

    auto const readByte = [&remainingSymbols]() noexcept -> std::uint8_t {
        auto const symbol = remainingSymbols[0U];
        remainingSymbols  = remainingSymbols.subspan(1U);
        return decodingTable[symbol];
    };

    auto buffer = output;

    std::array<std::uint8_t, 4U> bytes{};
    while (!remainingSymbols.empty())
    {
        for (auto &b : bytes)
        {
            b = readByte();
        }

        buffer = writeToSpan(buffer, static_cast<std::uint8_t>((bytes[0U] << 2) | (bytes[1U] >> 4)));
        buffer = writeToSpan(buffer, static_cast<std::uint8_t>((bytes[1U] << 4) | (bytes[2U] >> 2)));
        buffer = writeToSpan(buffer, static_cast<std::uint8_t>((bytes[2U] << 6) | (bytes[3U])));
    }
    return output.subspan(0U, output.size() - buffer.size() - equalSignsEnd);
}

Encoder::Encoder(gsl::span<char> outputBuffer) noexcept : _outputBuffer{outputBuffer}
{
    _accSpan = gsl::span<std::uint8_t>(_accumulator.data(), _accumulator.size());
}

bool Encoder::writeByte(std::uint8_t const byte) noexcept
{
    if (_outputBuffer.size() < 4U)
    {
        return false;
    }
    _accumulator[_bytesAccumulated] = byte;
    ++_bytesAccumulated;
    if (_bytesAccumulated == 3U)
    {
        _bytesAccumulated = 0U;
        encode(_accSpan, _outputBuffer);
        _outputBuffer = _outputBuffer.subspan(4U);
    }
    return true;
}

Decoder::Decoder(gsl::span<char const> inputBuffer) noexcept : _inputBuffer{inputBuffer}
{
    _accSpan = gsl::span<std::uint8_t>(_accumulator.data(), _accumulator.size());
}

bool Decoder::dataAvailable() const noexcept { return (_readIndex > 2U) || (_inputBuffer.size() > 3U); }

std::uint8_t Decoder::readByte() noexcept
{
    std::uint8_t result{};
    if (_readIndex > 2U)
    {
        if (_inputBuffer.size() > 3U)
        {
            _readIndex = 0U;
            decode(_inputBuffer.subspan(0U, 4U), _accSpan);
            _inputBuffer = _inputBuffer.subspan(4U);
        }
    }
    if (_readIndex < 3U)
    {
        result = _accumulator[_readIndex];
        ++_readIndex;
    }
    return result;
}

} // namespace Terrahertz::Base64
