#ifndef THZ_COMMON_CONFIGURATION_CONFIGURATIONBUILDER_HPP
#define THZ_COMMON_CONFIGURATION_CONFIGURATIONBUILDER_HPP

#include <string>
#include <string_view>
#include <vector>

namespace Terrahertz {

/// @brief Can be used to create example configurations.
class ConfigurationBuilder
{
public:
    /// @brief Creates a new ConfigruationBuilder instance.
    ConfigurationBuilder() noexcept;

    /// @brief Returns a builder for a subconfiguration, creating one if nonexistent.
    ///
    /// @param key The key for the subconfiguration.
    /// @param comment A comment to add to the subconfiguration.
    /// @return The builder for the sub configuration.
    ConfigurationBuilder &getCreateSubConfiguration(std::string_view key, std::string_view comment = "") noexcept;

    /// @brief Adds a configuration entry to the configuration.
    ///
    /// @param key The key of the entry.
    /// @param exampleValue An example value for the entry.
    /// @param comment A comment to add to the entry.
    void addEntry(std::string_view key, std::string_view exampleValue, std::string_view comment = "") noexcept;

    /// @brief Builds the example configuration string.
    ///
    /// @return A string containing the configuration example.
    std::string buildConfigurationString() const noexcept;

private:
    /// @brief Structure for an configuration entry.
    struct Entry
    {
        /// @brief The key of the entry.
        std::string_view key;

        /// @brief The example value for the entry.
        std::string_view exampleValue;

        /// @brief A comment to add to the entry.
        std::string_view comment;
    };

    /// @brief Builds the example configuration string for a sub configuration.
    ///
    /// @param prefix The prefix for the section string.
    /// @return A string containing the configuration example.
    std::string buildSubConfigurationString(std::string const prefix) const noexcept;

    /// @brief The key, if this is a subconfiguration.
    std::string_view _key;

    /// @brief The comment if this is a subconfiguration.
    std::string_view _comment;

    /// @brief The vector for the entries of this configuration.
    std::vector<Entry> _entries{};

    /// @brief The vector for subconfigurations of this configuration.
    std::vector<ConfigurationBuilder> _subConfigurations{};
};

} // namespace Terrahertz

#endif // !THZ_COMMON_CONFIGURATION_CONFIGURATIONBUILDER_HPP
