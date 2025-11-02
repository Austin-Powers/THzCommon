#include "THzCommon/utility/range2DFolding.hpp"

#include <limits>

namespace Terrahertz {

void Range2DFolding::Position::setup(size_t const newIndex, std::uint32_t const bufferWidth) noexcept
{
    if (bufferWidth != 0U)
    {
        index = newIndex;
        x     = static_cast<std::uint32_t>(index % bufferWidth);
        y     = static_cast<std::uint32_t>(index / bufferWidth);
    }
    else
    {
        index = std::numeric_limits<size_t>::max();
    }
}

Range2DFolding::Position::operator size_t() const noexcept { return index; }

Range2DFolding::InnerIterator::InnerIterator() noexcept { _index.index = std::numeric_limits<size_t>::max(); }

Range2DFolding::InnerIterator::InnerIterator(std::uint32_t const bufferWidth,
                                             std::uint32_t const zoneOffset,
                                             std::uint32_t const zoneWidth,
                                             size_t const        startIndex) noexcept
    : _bufferWidth{bufferWidth}, _xStart{zoneOffset}, _xStop{zoneOffset + zoneWidth}, _shift{bufferWidth - zoneWidth}
{
    // bufferWidth == 0U will be handled by _index.setup
    std::uint32_t const lineIndex = startIndex % ((bufferWidth == 0U) ? 1U : bufferWidth);
    if ((bufferWidth < (zoneOffset + zoneWidth)) ||
        ((lineIndex < zoneOffset) || (lineIndex >= (zoneOffset + zoneWidth))))
    {
        _index.index = std::numeric_limits<size_t>::max();
    }
    else
    {
        _index.setup(startIndex, _bufferWidth);
    }
}

Range2DFolding::InnerIterator::value_type Range2DFolding::InnerIterator::operator*() const noexcept { return _index; }

Range2DFolding::InnerIterator &Range2DFolding::InnerIterator::operator++() noexcept
{
    ++_index.index;
    ++_index.x;
    if (_index.x == _xStop)
    {
        _index.index += _shift;
        _index.x = _xStart;
        ++_index.y;
    }
    return *this;
}

Range2DFolding::InnerIterator Range2DFolding::InnerIterator::operator++(int) noexcept
{
    InnerIterator back = *this;
    ++(*this);
    return back;
}

bool Range2DFolding::InnerIterator::operator==(Range2DFolding::InnerIterator const &other) const noexcept
{
    return _index.index == other._index.index;
}

Range2DFolding::OuterIterator::OuterIterator() noexcept { _index.index = std::numeric_limits<size_t>::max(); }

Range2DFolding::OuterIterator::OuterIterator(std::uint32_t const bufferWidth,
                                             std::uint32_t const bufferHeight,
                                             std::uint32_t const zoneWidth,
                                             std::uint32_t const zoneHeight,
                                             std::uint32_t const shiftX,
                                             std::uint32_t const shiftY,
                                             size_t const        startIndex) noexcept
    : _bufferWidth{bufferWidth},
      _bufferHeight{bufferHeight},
      _zoneWidth{zoneWidth},
      _zoneHeight{zoneHeight},
      _shiftX{shiftX},
      _shiftY{shiftY}
{
    _index.setup(startIndex, bufferWidth);
    if ((_zoneWidth > _bufferWidth) || (_zoneHeight > _bufferHeight) || ((_index.x + _zoneWidth) > _bufferWidth) ||
        (_shiftX == 0U) || (_shiftY == 0U) || (_zoneWidth == 0U) || (_zoneHeight == 0U))
    {
        _index.index = std::numeric_limits<size_t>::max();
    }
}

Range2DFolding::OuterIterator::value_type const &Range2DFolding::OuterIterator::operator*() const noexcept
{
    return *this;
}

Range2DFolding::OuterIterator &Range2DFolding::OuterIterator::operator++() noexcept
{
    _index.index += _shiftX;
    _index.x += _shiftX;

    ++_targetIndex.index;
    ++_targetIndex.x;
    if ((_index.x + _zoneWidth) > _bufferWidth)
    {
        _index.x = 0U;
        _index.y += _shiftY;
        _index.index = static_cast<size_t>(_index.y) * _bufferWidth;

        _targetIndex.x = 0U;
        ++_targetIndex.y;
    }
    return *this;
}

Range2DFolding::OuterIterator Range2DFolding::OuterIterator::operator++(int) noexcept
{
    OuterIterator back = *this;
    ++(*this);
    return back;
}

bool Range2DFolding::OuterIterator::operator==(Range2DFolding::OuterIterator const &other) const noexcept
{
    return _index.index == other._index.index;
}

Range2DFolding::InnerIterator Range2DFolding::OuterIterator::begin() const noexcept
{
    return InnerIterator(_bufferWidth, _index.x, _zoneWidth, _index.x + (_index.y * _bufferWidth));
}

Range2DFolding::InnerIterator Range2DFolding::OuterIterator::end() const noexcept
{
    return InnerIterator(_bufferWidth, _index.x, _zoneWidth, _index.x + ((_index.y + _zoneHeight) * _bufferWidth));
}

Range2DFolding::Position Range2DFolding::OuterIterator::targetPosition() const noexcept { return _targetIndex; }

Range2DFolding::Range2DFolding(std::uint32_t const bufferWidth,
                               std::uint32_t const bufferHeight,
                               std::uint32_t const zoneWidth,
                               std::uint32_t const zoneHeight,
                               std::uint32_t const shiftX,
                               std::uint32_t const shiftY) noexcept
    : _bufferWidth{bufferWidth},
      _bufferHeight{bufferHeight},
      _zoneWidth{zoneWidth},
      _zoneHeight{zoneHeight},
      _shiftX{shiftX},
      _shiftY{shiftY}
{}

Range2DFolding::OuterIterator Range2DFolding::begin() const noexcept
{
    return OuterIterator{_bufferWidth, _bufferHeight, _zoneWidth, _zoneHeight, _shiftX, _shiftY};
}

Range2DFolding::OuterIterator Range2DFolding::end() const noexcept
{
    auto endIndex = 0U;
    if (_bufferHeight >= _zoneHeight)
    {
        auto const effectiveBufferHeight = _bufferHeight - _zoneHeight + 1U;
        while (endIndex < effectiveBufferHeight)
        {
            endIndex += _shiftY;
        }
        endIndex *= _bufferWidth;
    }
    return OuterIterator{_bufferWidth, _bufferHeight, _zoneWidth, _zoneHeight, _shiftX, _shiftY, endIndex};
}

std::pair<std::uint32_t, std::uint32_t> Range2DFolding::targetDimensions() const noexcept
{
    auto const oneDimension =
        [](std::uint32_t const buffer, std::uint32_t const zone, std::uint32_t const shift) noexcept -> std::uint32_t {
        if ((buffer == 0U) || (zone == 0U) || (shift == 0U) || (buffer < zone))
        {
            return 0U;
        }
        auto const effectiveValue = buffer - zone + shift;
        return effectiveValue / shift;
    };
    auto const width  = oneDimension(_bufferWidth, _zoneWidth, _shiftX);
    auto const height = oneDimension(_bufferHeight, _zoneHeight, _shiftY);
    if ((width == 0U) || (height == 0U))
    {
        return {0U, 0U};
    }
    return {width, height};
}

} // namespace Terrahertz
