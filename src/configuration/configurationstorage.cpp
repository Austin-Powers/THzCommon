#include "THzCommon/configuration/configurationstorage.hpp"

#include "THzCommon/logging/logging.hpp"
#include "THzCommon/utility/stringviewhelpers.hpp"

#include <fstream>
#include <map>

namespace Terrahertz {

ConfigurationStorage::ConfigurationStorage() noexcept { addProjectName<ConfigurationProject>(); }

bool ConfigurationStorage::load(std::string_view const buffer) noexcept
{
    std::string prefix = "";

    std::map<std::string, std::string> data{};
    for (auto line : StringViewTokenizer(buffer, '\n'))
    {
        // remove comments
        line = trim(split(line, ';').first);
        // skip empty lines
        if (line.empty())
        {
            continue;
        }
        // scan section
        auto startBracketPos = line.find('[');
        if (startBracketPos != std::string_view::npos)
        {
            startBracketPos += 1U;
            auto const endBracketPos = line.find(']');
            if (endBracketPos == std::string_view::npos)
            {
                logMessage<LogLevel::Error, ConfigurationProject>("Unable to parse configuration section:");
                logMessage<LogLevel::Error, ConfigurationProject>(line);
                return false;
            }
            prefix = line.substr(startBracketPos, endBracketPos - startBracketPos);
            prefix += ".";
        }
        else
        {
            // scan key-value-pair
            if (line.find('=') == std::string_view::npos)
            {
                return false;
            }
            auto const pair  = split(line, '=');
            auto const key   = prefix + std::string(trim(pair.first));
            auto const value = trim(pair.second);
            if (key.empty())
            {
                logMessage<LogLevel::Error, ConfigurationProject>("Key is missing:");
                logMessage<LogLevel::Error, ConfigurationProject>(line);
                return false;
            }
            data.emplace(key, value);
        }
    }
    _configuration = Configuration(data);
    return true;
}

bool ConfigurationStorage::load(std::filesystem::path const &location) noexcept
{
    std::ifstream configFile{location};
    if (!configFile.is_open())
    {
        return false;
    }
    std::string fileContent = "";
    for (std::string line; std::getline(configFile, line);)
    {
        fileContent += (line + "\n");
    }
    return load(std::string_view(fileContent));
}

Configuration const &ConfigurationStorage::configuration() const noexcept { return _configuration; }

} // namespace Terrahertz
