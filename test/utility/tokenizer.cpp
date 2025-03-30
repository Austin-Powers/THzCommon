#include "THzCommon/utility/tokenizer.hpp"

#include <gtest/gtest.h>
#include <string_view>

namespace Terrahertz::UnitTests {

struct UtilityTokenizer : public testing::Test
{};

TEST_F(UtilityTokenizer, StringViewTokenizerWorksAsExpected)
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

} // namespace Terrahertz::UnitTests
