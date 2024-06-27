#ifdef _WIN32

#include "THzCommon/network/socketwin32.hpp"

#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct NetworkSocketWin32 : public testing::Test
{};

TEST_F(NetworkSocketWin32, InitDeinit) { SocketWin32 sut{}; }

} // namespace Terrahertz::UnitTests

#endif // !_WIN32
