#include "THzCommon/configuration/configuration.hpp"

#include <iostream>

namespace Terrahertz {

Configuration::Configuration(std::map<std::string, std::string> const &buffer) noexcept
{
    for (auto const &kvPair : buffer)
    {
        addEntry(kvPair.first, kvPair.second);
    }
}

std::string_view Configuration::valueOf(std::string const key) const noexcept
{
    std::string_view result{};

    auto const valueIter = _leaves.find(key);
    if (valueIter != _leaves.end())
    {
        result = (*valueIter).second;
    }
    return result;
}

Configuration const *Configuration::subConfiguration(std::string const key) const noexcept
{
    Configuration const *result{};

    auto const subIter = _branches.find(key);
    if (subIter != _branches.end())
    {
        result = &(subIter->second);
    }
    return result;
}

void Configuration::addEntry(std::string const &key, std::string const &value) noexcept
{
    auto const pos = key.find('.');
    if (pos == std::string::npos)
    {
        // value
        _leaves.emplace(key, value);
    }
    else
    {
        // sub configuration
        auto const subkey = key.substr(0U, pos);
        auto       iter   = _branches.find(subkey);
        if (iter == _branches.end())
        {
            _branches.emplace(subkey, Configuration());
            iter = _branches.find(subkey);
        }
        (*iter).second.addEntry(key.substr(pos + 1U), value);
    }
}

} // namespace Terrahertz
