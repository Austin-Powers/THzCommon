#include "THzCommon/converter/huffmancoder.hpp"

#include "THzCommon/logging/logging.hpp"
#include "THzCommon/structures/stack.hpp"
#include "THzCommon/utility/spanhelpers.hpp"

#include <bit>
#include <cstring>

namespace Terrahertz::Huffman {

bool Encoder::compress(gsl::span<std::uint8_t const> data) noexcept
{
    if (!_data.empty())
    {
        return false;
    }
    _data = data;
    SymbolDistribution distribution{};
    for (auto const symbol : _data)
    {
        ++distribution[symbol];
    }
    _table.create(distribution);
    _headerWritten = false;
    _expectedSize  = sizeof(CodeHeader) + _table.calculateExpectation(distribution);
    _leftoverBits  = 0U;
    return true;
}

size_t Encoder::expectedSize() const noexcept { return _expectedSize; }

size_t Encoder::collectCompressedData(gsl::span<std::uint8_t> const buffer) noexcept
{
    if (buffer.empty())
    {
        logMessage<LogLevel::Warning, HuffmanProject>("Encoder: buffer is empty");
        return 0U;
    }
    if (_data.empty())
    {
        return 0U;
    }
    auto remainingBuffer = buffer;
    if (!_headerWritten)
    {
        CodeHeader header{};
        header.signature            = CodeSignature;
        header.uncompressedDataSize = _data.size();
        header.compressedDataSize   = _expectedSize - sizeof(CodeHeader);
        if (buffer.size() < sizeof(CodeHeader))
        {
            logMessage<LogLevel::Error, HuffmanProject>("Encoder: buffer too small for CodeHeader");
            return 0U;
        }
        remainingBuffer = writeToSpan(remainingBuffer, header);
        auto tempBuffer = _table.write(remainingBuffer);
        if (remainingBuffer.size() == tempBuffer.size())
        {
            logMessage<LogLevel::Error, HuffmanProject>("Encoder: buffer too small for CodeTable");
            return 0U;
        }
        remainingBuffer = tempBuffer;
        _headerWritten  = true;
    }

    BitBufferWriter writer{remainingBuffer};
    if (_leftoverBits != 0)
    {
        // write leftovers from last symbol
        _leftoverBits = _table.encode(writer, _data[0U], _leftoverBits);
        if (_leftoverBits == -1)
        {
            logMessage<LogLevel::Error, HuffmanProject>("Encoder: unexpected symbol");
            return 0U;
        }
        else if (_leftoverBits > 0)
        {
            logMessage<LogLevel::Warning, HuffmanProject>(
                "Encoder: writing leftovers left leftovers, consider using a bigger buffer");
            return buffer.size();
        }
        _data = _data.subspan(1);
    }
    while (!_data.empty())
    {
        _leftoverBits = _table.encode(writer, _data[0U]);
        if (_leftoverBits == -1)
        {
            logMessage<LogLevel::Error, HuffmanProject>("Encoder: unexpected symbol");
            return 0U;
        }
        else if (_leftoverBits > 0)
        {
            break;
        }
        _data = _data.subspan(1);
    }

    return static_cast<size_t>(buffer.size() - (writer.bitsLeft() / 8U));
}

bool Decoder::decompress(gsl::span<std::uint8_t const> buffer) noexcept
{
    if (buffer.size() < sizeof(CodeHeader))
    {
        logMessage<LogLevel::Warning, HuffmanProject>("Decoder: buffer too small for CodeHeader");
        return false;
    }
    CodeHeader header{};
    buffer = readFromSpan(buffer, header);
    if (header.signature != CodeSignature)
    {
        logMessage<LogLevel::Warning, HuffmanProject>("Decoder: CodeHeader has the wrong signature");
        return false;
    }
    if (static_cast<size_t>(buffer.size()) < header.compressedDataSize)
    {
        logMessage<LogLevel::Warning, HuffmanProject>("Decoder: Buffer to small for compressed data");
        return false;
    }
    _bytesLeft = header.uncompressedDataSize;

    auto tempBuffer = _table.read(buffer);
    if (buffer.size() == tempBuffer.size())
    {
        logMessage<LogLevel::Warning, HuffmanProject>("Decoder: Failed to read the CodeTable");
        return false;
    }
    _reader = BitBufferReader{tempBuffer};
    return true;
}

size_t Decoder::collectDecompressedData(gsl::span<std::uint8_t> buffer) noexcept
{
    size_t bytesRead{};
    for (; (_bytesLeft != 0U) && (!buffer.empty()); buffer = buffer.subspan(1U))
    {
        buffer[0] = collectNextByte();
        ++bytesRead;
    }
    return bytesRead;
}

std::uint8_t Decoder::collectNextByte() noexcept
{
    if (_bytesLeft == 0U)
    {
        return 0U;
    }
    --_bytesLeft;
    return _table.decode(_reader);
}

size_t Decoder::bytesLeft() const noexcept { return _bytesLeft; }

} // namespace Terrahertz::Huffman
