#include "THzCommon/network/common.hpp"

namespace Terrahertz {
namespace Detail {

SocketApi::SocketApi() = default;

SocketApi::~SocketApi() = default;

SocketApi const &SocketApi::instance()
{
    static SocketApi api{};
    return api;
}

} // namespace Detail
} // namespace Terrahertz
