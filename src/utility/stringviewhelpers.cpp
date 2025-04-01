#include "THzCommon/utility/stringviewhelpers.hpp"

#include <cctype>
#include <cstddef>

namespace Terrahertz {

StringViewTokenizer::Iterator::Iterator(std::string_view const &view, char const delimiter) noexcept
    : _view{view}, _delimiter{delimiter}
{}

std::string_view StringViewTokenizer::Iterator::operator*() const noexcept
{
    return _view.substr(0, _view.find(_delimiter));
}

StringViewTokenizer::Iterator &StringViewTokenizer::Iterator::operator++() noexcept
{
    auto const pos = _view.find(_delimiter) + 1U;
    if (pos == 0U)
    {
        _view = "";
    }
    else
    {
        _view = _view.substr(pos);
    }
    return *this;
}

StringViewTokenizer::Iterator StringViewTokenizer::Iterator::operator++(int) noexcept
{
    Iterator back = *this;
    ++(*this);
    return back;
}

bool StringViewTokenizer::Iterator::operator==(Iterator const &other) const noexcept
{
    return _view.size() == other._view.size();
}

StringViewTokenizer::StringViewTokenizer(std::string_view const &view, char const delimiter) noexcept
    : _view{view}, _delimiter{delimiter}
{}

StringViewTokenizer::Iterator StringViewTokenizer::begin() const noexcept { return Iterator(_view, _delimiter); }

StringViewTokenizer::Iterator StringViewTokenizer::end() const noexcept { return Iterator(); }

std::pair<std::string_view, std::string_view> split(std::string_view const toSplit, char const delimiter) noexcept
{
    auto const position = toSplit.find(delimiter);
    if (position == std::string_view::npos)
    {
        return {toSplit.substr(0U, position), ""};
    }
    return {toSplit.substr(0U, position), toSplit.substr(position + 1U)};
}

std::string_view trim(std::string_view view) noexcept
{
    size_t off  = 0U;
    size_t size = view.size();
    while ((size != 0U) && (std::isspace(view[off]) != 0))
    {
        ++off;
        --size;
    }
    while ((size > 0U) && (std::isspace(view[off + size - 1U]) != 0))
    {
        --size;
    }
    return view.substr(off, size);
}

} // namespace Terrahertz
