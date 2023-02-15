#include "THzCommon/utility/range2D.hpp"

namespace Terrahertz {

Range2D::Position::operator size_t() const noexcept { return index; }

Range2D::Iterator::Iterator(std::uint32_t const width, size_t const startIndex) noexcept : _width{width}
{
    _index.index = startIndex;
    if (_width != 0U)
    {
        _index.x = static_cast<std::uint32_t>(startIndex % _width);
        _index.y = static_cast<std::uint32_t>(startIndex / _width);
    }
}

Range2D::Iterator::value_type Range2D::Iterator::operator*() const noexcept { return _index; }

Range2D::Iterator &Range2D::Iterator::operator++() noexcept
{
    ++_index.index;
    ++_index.x;
    if (_index.x == _width)
    {
        _index.x = 0U;
        ++_index.y;
    }
    return *this;
}

Range2D::Iterator Range2D::Iterator::operator++(int) noexcept
{
    Iterator back = *this;
    ++(*this);
    return back;
}

bool Range2D::Iterator::operator==(Range2D::Iterator const &other) const noexcept
{
    return _index.index == other._index.index;
}

Range2D::Range2D(std::uint32_t width, std::uint32_t height) noexcept : _width{width}, _height{height} {}

Range2D::Iterator Range2D::begin() const noexcept { return Iterator{_width}; }

Range2D::Iterator Range2D::end() const noexcept { return Iterator{_width, _width * _height}; }

} // namespace Terrahertz
