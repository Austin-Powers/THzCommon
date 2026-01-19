#include "THzCommon/utility/stringhelpers.hpp"

#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct UtilityStringHelpers : public testing::Test
{};

TEST_F(UtilityStringHelpers, toLowerInplace)
{
    std::string input = "aAbBcCdD";
    toLowerInplace(input);
    EXPECT_EQ(input, "aabbccdd");
}

TEST_F(UtilityStringHelpers, toUpperInplace)
{
    std::string input = "aAbBcCdD";
    toUpperInplace(input);
    EXPECT_EQ(input, "AABBCCDD");
}

TEST_F(UtilityStringHelpers, toLower)
{
    std::string const input  = "aAbBcCdD";
    std::string const output = toLower(input);
    EXPECT_EQ(output, "aabbccdd");
}

TEST_F(UtilityStringHelpers, toUpper)
{
    std::string const input  = "aAbBcCdD";
    std::string const output = toUpper(input);
    EXPECT_EQ(output, "AABBCCDD");
}

} // namespace Terrahertz::UnitTests
