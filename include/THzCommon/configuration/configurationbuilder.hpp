#ifndef THZ_COMMON_CONFIGURATION_CONFIGURATIONBUILDER_HPP
#define THZ_COMMON_CONFIGURATION_CONFIGURATIONBUILDER_HPP

#include <string_view>

namespace Terrahertz {

/// @brief Can be used to create example configurations.
class ConfigurationBuilder
{
public:
    /// @brief Creates a new ConfigruationBuilder instance.
    ConfigurationBuilder() noexcept;

    /// @brief
    ///
    /// @param key
    /// @param comment
    /// @return
    ConfigurationBuilder createConfiguration(std::string_view key, std::string_view comment) noexcept;

    void addEntry(std::string_view key, std::string_view exampleValue, std::string_view comment) noexcept;

    std::string buildConfigurationString() const noexcept;

private:
};

} // namespace Terrahertz

#endif // !THZ_COMMON_CONFIGURATION_CONFIGURATIONBUILDER_HPP
