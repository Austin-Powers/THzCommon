#ifndef THZ_COMMON_NETWORK_TCPSOCKET_HPP
#define THZ_COMMON_NETWORK_TCPSOCKET_HPP

#include "tcpsocketposix.hpp"
#include "tcpsocketwin32.hpp"

namespace Terrahertz {

#ifdef _WIN32
/// @brief A network socket using TCP.
using TCPSocket = TCPSocketWin32;
#else
using TCPSocket = TCPSocketPosix;
#endif // !_WIN32

} // namespace Terrahertz

#endif // !THZ_COMMON_NETWORK_TCPSOCKET_HPP
