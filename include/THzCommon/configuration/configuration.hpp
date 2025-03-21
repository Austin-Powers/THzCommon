#include <map>
#include <set>
#include <string>

namespace Terrahertz {

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

    /// @brief Checks if a certain key is present in this configuration.
    ///
    /// @param key The key to lookup.
    /// @return True if the key is present, false otherwise.
    bool isPresent(std::string const key) const noexcept;

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
    /// @brief Initializes a sub configuration instance.
    ///
    /// @param buffer The map containing the configuration values.
    /// @param prefix The prefix to put on all keys.
    Configuration(std::map<std::string, std::string> const &buffer, std::string const prefix) noexcept;

    /// @brief The prefix put on all keys.
    std::string _prefix{};

    /// @brief Pointer to the map containing the configuration values.
    std::map<std::string, std::string> const *_buffer{};

    /// @brief Set containing all entries of the configuration.
    std::set<std::string> _entries{};
};

} // namespace Terrahertz
