#include "THzCommon/configuration/configurationstorage.hpp"

namespace Terrahertz {

ConfigurationStorage::ConfigurationStorage() noexcept {}

bool ConfigurationStorage::load(std::string_view const buffer) noexcept { return false; }

bool ConfigurationStorage::load(std::filesystem::path const &location) noexcept { return false; }

Configuration const &ConfigurationStorage::configuration() const noexcept { return _configuration; }

} // namespace Terrahertz
