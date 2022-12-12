#include "THzCommon/utility/staticPImpl.h"

#include "sptesthelper.h"

#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct Utility_StaticPImpl : public testing::Test
{};

TEST_F(Utility_StaticPImpl, DefaultInitialization)
{
    DefaultInitTestHelper helper{};
    EXPECT_EQ(helper.foo(), -128);
}

TEST_F(Utility_StaticPImpl, InitializationWithParameters)
{
    StaticPImplTestHelper helper{22};
    EXPECT_EQ(helper.getNumber(), 22);
}

} // namespace Terrahertz::UnitTests
