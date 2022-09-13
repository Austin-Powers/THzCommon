#include "THzCommon/structures/queue.h"

#include <cstdint>
#include <gtest/gtest.h>

namespace Terrahertz::UnitTests {

struct Structures_Queue : public testing::Test
{
    using TestQueue = Queue<std::uint32_t, 8U>;

    TestQueue sut{};
};

TEST_F(Structures_Queue, Test) {}

} // namespace Terrahertz::UnitTests
