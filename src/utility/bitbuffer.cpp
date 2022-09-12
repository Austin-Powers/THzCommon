#include "BitBuffer.h"

#include "Logging/Logging.h"

#include <cstring>

namespace Terrahertz {

BitBufferReader::BitBufferReader(gsl::span<std::uint8_t const> buffer) noexcept : _buffer{buffer} {}

size_t BitBufferReader::bitsLeft() const noexcept { return _buffer.size() * 8U + _bitsLeftInByte; }

bool BitBufferReader::next() noexcept
{
    if (_bitsLeftInByte == 0U)
    {
        if (_buffer.empty())
        {
            return false;
        }
        else
        {
            _nextByte       = _buffer[0];
            _buffer         = _buffer.subspan(1);
            _bitsLeftInByte = 8U;
        }
    }

    auto const result = (_nextByte & 0b1000'0000U) != 0U;
    _nextByte <<= 1U;
    --_bitsLeftInByte;
    return result;
}

BitBufferWriter::BitBufferWriter(gsl::span<std::uint8_t> buffer) noexcept : _buffer{buffer} {}

size_t BitBufferWriter::bitsLeft() const noexcept { return (_buffer.size() * 8U) - _nextBitInByte; }

bool BitBufferWriter::write(bool const bit) noexcept
{
    if (bitsLeft() == 0U)
    {
        return false;
    }
    if (bit)
    {
        _buffer[0] |= _mask;
    }
    else
    {
        _buffer[0] &= ~_mask;
    }

    _mask >>= 1U;
    ++_nextBitInByte;
    if (_mask == 0U)
    {
        _mask          = 0b1000'0000;
        _nextBitInByte = 0U;
        _buffer        = _buffer.subspan(1);
    }
    return true;
}

size_t BitBufferWriter::write(gsl::span<std::uint8_t const> bytes, size_t const count) noexcept
{
    if (static_cast<size_t>(bytes.size()) < ((count + 7U) / 8U))
    {
        return 0U;
    }

    size_t written{};
    for (auto nextByte : bytes)
    {
        for (auto i = 0U; i < 8U; ++i)
        {
            if (!write((nextByte & 0x80) == 0x80))
            {
                return written;
            }
            nextByte <<= 1U;
            ++written;
            if (written == count)
            {
                return written;
            }
        }
    }
    return written;
}

size_t BitBufferWriter::write(gsl::span<std::uint8_t const> bytes, size_t const count, size_t offset) noexcept
{
    if (static_cast<size_t>(bytes.size()) < ((count + 7U) / 8U))
    {
        return 0U;
    }

    size_t written{};
    for (auto nextByte : bytes)
    {
        for (auto i = 0U; i < 8U; ++i)
        {
            if (offset != 0U)
            {
                nextByte <<= 1U;
                --offset;
                continue;
            }
            if (!write((nextByte & 0x80) == 0x80))
            {
                return written;
            }
            nextByte <<= 1U;
            ++written;
            if (written == count)
            {
                return written;
            }
        }
    }
    return written;
}

} // namespace Terrahertz
