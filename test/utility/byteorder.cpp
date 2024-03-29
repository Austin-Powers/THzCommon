#include "THzCommon/utility/byteorder.hpp"

#include <cstdint>
#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct UtilityByteOrder : public testing::Test
{};

TEST_F(UtilityByteOrder, UInt32)
{
    std::uint32_t value{0x12345678U};
    EXPECT_EQ(flipByteOrder(value), 0x78563412U);
}

} // namespace Terrahertz::UnitTests
