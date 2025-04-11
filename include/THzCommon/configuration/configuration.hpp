#ifndef THZ_COMMON_CONFIGURATION_CONFIGURATION_HPP
#define THZ_COMMON_CONFIGURATION_CONFIGURATION_HPP

#include <map>
#include <set>
#include <string>

namespace Terrahertz {

/// @brief Name provider for the configuration project.
struct ConfigurationProject
{
    static constexpr char const *name() noexcept { return "THzCommon.Configuration"; }
};

/// @brief Manages access to a tree like configuration structure.
class Configuration
{
public:
    /// @brief Default initializes a new configuration.
    Configuration() noexcept = default;

    /// @brief Initializes a new configuration instance.
    ///
    /// @param configurationMap The map containing the configuration values.
    Configuration(std::map<std::string, std::string> const &buffer) noexcept;

    /// @brief Retrieves the value for the given key.
    ///
    /// @param key The key to of the value.
    /// @return The value, if the key exists and is linked to a value.
    std::string_view valueOf(std::string const key) const noexcept;

    /// @brief Retrieves the sub configuration for the given key.
    ///
    /// @param key The key to the sub configuration.
    /// @return The sub configuration, if the key exists and is linked to a value.
    Configuration const *subConfiguration(std::string const key) const noexcept;

private:
    /// @brief Adds the given key-value-pair to the configuration.
    ///
    /// @param key The key of to find the value by.
    /// @param value The value to the key.
    void addEntry(std::string const &key, std::string const &value) noexcept;

    /// @brief Map of sub configurations.
    std::map<std::string, Configuration> _branches{};

    /// @brief Map of values.
    std::map<std::string, std::string> _leaves{};
};

} // namespace Terrahertz

#endif // !THZ_COMMON_CONFIGURATION_CONFIGURATION_HPP
