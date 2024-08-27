#include "THzCommon/network/common.hpp"

namespace Terrahertz {
namespace Native {

SocketApi::SocketApi() = default;

SocketApi::~SocketApi() = default;

SocketApi const &SocketApi::instance()
{
    static SocketApi api{};
    return api;
}

} // namespace Native
} // namespace Terrahertz
