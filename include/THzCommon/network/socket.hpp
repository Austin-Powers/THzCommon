#ifndef THZ_COMMON_NETWORK_SOCKET_HPP
#define THZ_COMMON_NETWORK_SOCKET_HPP

#include "socketposix.hpp"
#include "socketwin32.hpp"

namespace Terrahertz {

#ifdef _WIN32
/// @brief A network socket using TCP.
using Socket = SocketWin32;
#else
using Socket = SocketPosix;
#endif // !_WIN32

} // namespace Terrahertz

#endif // !THZ_COMMON_NETWORK_SOCKET_HPP
