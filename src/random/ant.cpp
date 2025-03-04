#include "THzCommon/random/ant.hpp"

#include "THzCommon/converter/base64.hpp"

#include <iostream>
#include <random>

namespace Terrahertz {

Ant::Ant() noexcept
{
    std::random_device rd{};
    std::mt19937       randomEngine{rd()};

    std::uniform_int_distribution<std::uint16_t> distr{0U, 63U};
    _posX      = distr(randomEngine);
    _posY      = distr(randomEngine);
    _direction = distr(randomEngine) & 0x3U;
    // Half of the sequence needs to be Right so the output is balanced
    for (auto r = 0U; r < (_sequence.size() / 2U);)
    {
        auto const idx = distr(randomEngine) & 0xFU;
        if (_sequence[idx] == 0U)
        {
            _sequence[idx] = 1U;
            ++r;
        }
    }
    _step = 0U;
    for (auto &cell : _grid)
    {
        cell = distr(randomEngine) & 0xFU;
    }
}

/// @brief The size of the encoded data [bytes].
constexpr std::uint16_t ENCODEDSIZE = 2744U;

bool Ant::load(gsl::span<char const> const encodedData) noexcept
{
    if (encodedData.size() < ENCODEDSIZE)
    {
        return false;
    }
    Base64::Decoder decoder{encodedData};
    _posX      = decoder.readByte();
    _posY      = decoder.readByte();
    _direction = decoder.readByte();
    std::uint16_t sequence{};
    decoder.read(sequence);
    for (auto &part : _sequence)
    {
        part = sequence >> 0xFU;
        sequence <<= 1U;
    }
    decoder.read(_step);
    decoder.readByte();
    for (auto i = 0U; i < _grid.size(); i += 2U)
    {
        auto const packedCells = decoder.readByte();

        _grid[i]        = (packedCells >> 4U);
        _grid[i + 1ULL] = (packedCells & 0xFU);
    }
    return true;
}

gsl::span<char> Ant::save(gsl::span<char> buffer) noexcept
{
    if (buffer.size() < ENCODEDSIZE)
    {
        return {};
    }
    Base64::Encoder encoder{buffer};
    encoder.writeByte(_posX);
    encoder.writeByte(_posY);
    encoder.writeByte(_direction);
    std::uint16_t sequence{};
    for (auto const part : _sequence)
    {
        sequence <<= 1U;
        sequence |= part;
    }
    encoder.write(sequence);
    encoder.write(_step);
    // Add one byte of padding to make handling easier
    encoder.writeByte(0U);
    for (auto i = 0U; i < _grid.size(); i += 2U)
    {
        std::uint8_t packedCells = (_grid[i] << 4U) | _grid[i + 1ULL];
        encoder.writeByte(packedCells);
    }
    return buffer.subspan(0U, ENCODEDSIZE);
}

std::uint32_t Ant::step() const noexcept { return _step; }

void Ant::syncTo(std::uint32_t const step) noexcept
{
    while (_step > step)
    {
        backward();
    }
    while (_step < step)
    {
        forward();
    }
}

std::uint8_t Ant::nextByte() noexcept
{
    std::uint8_t result{};
    for (auto i = 0U; i < 8U; ++i)
    {
        result <<= 1U;
        if (forward())
        {
            result |= 1U;
        }
    }
    return result;
}

bool Ant::forward() noexcept
{
    auto &cell = _grid[(GRIDLENGTH * _posY) + _posX];
    // rotate
    auto const turnRight = _sequence[cell] == 0U;

    _direction = (turnRight ? ++_direction : --_direction) & 0x3U;
    // change color
    cell = (cell + 1U) & 0xFU;
    // move
    switch (_direction)
    {
    case 0: // Up
        _posY = (_posY - 1U) & 0x3FU;
        break;
    case 1: // Right
        _posX = (_posX + 1U) & 0x3FU;
        break;
    case 2: // Down
        _posY = (_posY + 1U) & 0x3FU;
        break;
    case 3: // Left
        _posX = (_posX - 1U) & 0x3FU;
        break;
    }
    ++_step;
    return turnRight;
}

void Ant::backward() noexcept
{
    --_step;
    // move
    switch (_direction)
    {
    case 0: // Up
        _posY = (_posY + 1U) & 0x3FU;
        break;
    case 1: // Right
        _posX = (_posX - 1U) & 0x3FU;
        break;
    case 2: // Down
        _posY = (_posY - 1U) & 0x3FU;
        break;
    case 3: // Left
        _posX = (_posX + 1U) & 0x3FU;
        break;
    }
    auto &cell = _grid[(GRIDLENGTH * _posY) + _posX];
    // change color
    cell = (cell - 1U) & 0xFU;
    // rotate
    _direction = ((_sequence[cell] == 0U) ? --_direction : ++_direction) & 0x3U;
}

} // namespace Terrahertz
