#include "THzCommon/configuration/configurationbuilder.hpp"

namespace Terrahertz {

ConfigurationBuilder::ConfigurationBuilder() noexcept {}

ConfigurationBuilder &ConfigurationBuilder::getCreateSubConfiguration(std::string_view const key,
                                                                      std::string_view const comment) noexcept
{
    for (auto &sub : _subConfigurations)
    {
        if (sub._key == key)
        {
            return sub;
        }
    }
    auto &newSub    = _subConfigurations.emplace_back();
    newSub._key     = key;
    newSub._comment = comment;
    return newSub;
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

std::string ConfigurationBuilder::buildConfigurationString() const noexcept { return buildSubConfigurationString(""); }

std::string ConfigurationBuilder::buildSubConfigurationString(std::string const prefix) const noexcept
{
    std::string result = "";

    // entries
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
    // subconfigurations
    for (auto const &sub : _subConfigurations)
    {
        result += "[";
        result += prefix;
        result += sub._key;
        result += "]";
        if (!sub._comment.empty())
        {
            result += ";";
            result += sub._comment;
        }
        result += "\n";
        std::string subPrefix = prefix;
        subPrefix += sub._key;
        subPrefix += ".";
        result += sub.buildSubConfigurationString(subPrefix);
    }

    if (result.empty())
    {
        result += "\n";
    }
    return result;
}

} // namespace Terrahertz
