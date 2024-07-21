#include "THzCommon/network/native.hpp"

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
