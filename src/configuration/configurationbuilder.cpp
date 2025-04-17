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
    _entries.emplace_back(key, exampleValue, comment);
}

std::string ConfigurationBuilder::buildConfigurationString() const noexcept
{
    std::string result = "";
    for (auto const &entry : _entries)
    {}
    return "\n";
}

} // namespace Terrahertz
