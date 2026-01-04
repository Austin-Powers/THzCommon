#include "THzCommon/network/common.hpp"

namespace Terrahertz {
namespace Internal {

SocketApi::SocketApi() = default;

SocketApi::~SocketApi() noexcept = default;

SocketApi const &SocketApi::instance()
{
    static SocketApi api{};
    return api;
}

} // namespace Internal
} // namespace Terrahertz
