#include "THzCommon/utility/stringviewhelpers.hpp"

#include <gtest/gtest.h>
#include <string_view>

namespace Terrahertz::UnitTests {

struct UtilityStringViewHelpers : public testing::Test
{};

TEST_F(UtilityStringViewHelpers, StringViewTokenizerWorksAsExpected)
{
    std::uint16_t    index = 0U;
    std::string_view view  = "test:this:stuff:now";
    for (auto part : StringViewTokenizer(view, ':'))
    {
        switch (index)
        {
        case 0:
            EXPECT_TRUE(part == "test");
            break;
        case 1:
            EXPECT_TRUE(part == "this");
            break;
        case 2:
            EXPECT_TRUE(part == "stuff");
            break;
        case 3:
            EXPECT_TRUE(part == "now");
            break;
        default:
            FAIL();
            break;
        }
        ++index;
    }
}

TEST_F(UtilityStringViewHelpers, Trim)
{
    std::string_view empty    = "";
    std::string_view only     = " \t\r\n";
    std::string_view front    = " \r\n\ttest";
    std::string_view back     = "test \t\r\n";
    std::string_view both     = " \r\n\ttest \t\r\n";
    std::string_view inside   = "te\r\n\t st";
    std::string_view expected = "test";

    EXPECT_TRUE(empty == trim(empty));
    EXPECT_TRUE(empty == trim(only));
    EXPECT_TRUE(expected == trim(front));
    EXPECT_TRUE(expected == trim(back));
    EXPECT_TRUE(expected == trim(both));
    EXPECT_TRUE(inside == trim(inside));
}

} // namespace Terrahertz::UnitTests
