#include "THzCommon/configuration/configuration.hpp"

#include <filesystem>
#include <string_view>

namespace Terrahertz {

/// @brief Loads a configuration from a file or memorybuffer and provides it to the rest of the application.
///
/// @remark The configuration needs to be provided in the INI file format like:
/// ;comment
/// [section]
/// key = value;other comment
/// other_key=other_value
///
/// [section.subsection]
/// subkey = subvalue
///
class ConfigurationStorage
{
public:
    /// @brief Initialies a new empty storage.
    ConfigurationStorage() noexcept;

    /// @brief Loads a configuration from the given memory buffer.
    ///
    /// @param buffer The buffer to load the data from.
    /// @return True if the configuration was loaded successfully, false otherwise.
    /// @remark The currently loaded configuration will be overwritten.
    bool load(std::string_view const buffer) noexcept;

    /// @brief Loads a configuration from the file at the given location.
    ///
    /// @param location The location of the configuration file.
    /// @return True if the configuration was loaded successfully, false otherwise.
    /// @remark The currently loaded configuration will be overwritten.
    bool load(std::filesystem::path const &location) noexcept;

    /// @brief Returns the currently stored configuration.
    ///
    /// @return The currently stored configuration.
    Configuration const &configuration() const noexcept;

private:
    /// @brief The currently stored configuration.
    Configuration _configuration{};
};

} // namespace Terrahertz
