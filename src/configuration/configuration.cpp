#include "THzCommon/configuration/configuration.hpp"

#include <iostream>

namespace Terrahertz {

Configuration::Configuration(std::map<std::string, std::string> const &buffer) noexcept : _prefix{""}, _buffer{&buffer}
{
    for (auto const &kvPair : buffer)
    {
        auto const &key = kvPair.first;
        for (auto pos = key.find('.'); pos != std::string::npos; pos = key.find('.', pos + 1U))
        {
            _entries.insert(key.substr(0U, pos));
        }
        _entries.insert(key);
    }
}

bool Configuration::isPresent(std::string const key) const noexcept { return _entries.contains(key); }

std::string_view Configuration::valueOf(std::string const key) const noexcept
{
    std::string_view result{};
    if (_buffer)
    {
        auto const valueIter = _buffer->find(key);
        if (valueIter != _buffer->end())
        {
            result = (*valueIter).second;
        }
    }
    return result;
}

Configuration const *Configuration::subConfiguration(std::string const key) const noexcept { return nullptr; }

Configuration::Configuration(std::map<std::string, std::string> const &buffer, std::string const prefix) noexcept {}

} // namespace Terrahertz
