#include "THzCommon/configuration/configurationbuilder.hpp"

namespace Terrahertz {

ConfigurationBuilder::ConfigurationBuilder() noexcept {}

ConfigurationBuilder ConfigurationBuilder::createSubConfiguration(std::string_view const key,
                                                                  std::string_view const comment) noexcept
{
    return {};
}

void ConfigurationBuilder::addEntry(std::string_view const key,
                                    std::string_view const exampleValue,
                                    std::string_view const comment) noexcept
{
    bool found = false;
    for (auto &entry : _entries)
    {
        if (entry.key == key)
        {
            entry.exampleValue = exampleValue;
            entry.comment      = comment;
            found              = true;
            break;
        }
    }
    if (!found)
    {
        _entries.emplace_back(key, exampleValue, comment);
    }
}

std::string ConfigurationBuilder::buildConfigurationString() const noexcept
{
    std::string result = "";
    for (auto const &entry : _entries)
    {
        result += entry.key;
        result += " = ";
        result += entry.exampleValue;
        if (!entry.comment.empty())
        {
            result += ";";
            result += entry.comment;
        }
        result += "\n";
    }
    if (result.empty())
    {
        result += "\n";
    }
    return result;
}

} // namespace Terrahertz
