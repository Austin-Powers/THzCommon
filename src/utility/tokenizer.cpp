#include "THzCommon/utility/tokenizer.hpp"

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

} // namespace Terrahertz
