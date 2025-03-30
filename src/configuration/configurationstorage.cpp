#include "THzCommon/configuration/configurationstorage.hpp"

#include "THzCommon/utility/tokenizer.hpp"

#include <iostream>
#include <map>

namespace Terrahertz {

ConfigurationStorage::ConfigurationStorage() noexcept {}

bool ConfigurationStorage::load(std::string_view const buffer) noexcept
{
    std::string prefix = "";

    std::map<std::string, std::string> data{};
    for (auto line : StringViewTokenizer(buffer, '\n'))
    {
        std::cout << line << std::endl;
    }
    _configuration = Configuration(data);
    return false;
}

bool ConfigurationStorage::load(std::filesystem::path const &location) noexcept { return false; }

Configuration const &ConfigurationStorage::configuration() const noexcept { return _configuration; }

} // namespace Terrahertz
