#include "THzCommon/structures/octree.hpp"

#include <cstdint>
#include <gtest/gtest.h>
#include <random>

namespace Terrahertz::UnitTests {

struct Structures_Octree : public testing::Test
{
    using TestOctreeType = Octree<std::uint16_t, std::uint32_t, std::int32_t>;

    TestOctreeType sut{128U, 128U, 128U, 256U, 4U, 8U};

    void checkTree(TestOctreeType const &tree,
                   std::uint16_t const   centerX,
                   std::uint16_t const   centerY,
                   std::uint16_t const   centerZ,
                   std::uint32_t const   size,
                   std::uint32_t const   maxEntries,
                   std::uint8_t const    maxDepth,
                   size_t const          totalEntries) noexcept
    {

        EXPECT_EQ(tree.getCenterX(), centerX);
        EXPECT_EQ(tree.getCenterY(), centerY);
        EXPECT_EQ(tree.getCenterZ(), centerZ);
        EXPECT_EQ(tree.getSize(), size);
        EXPECT_EQ(tree.getMaxEntries(), maxEntries);
        EXPECT_EQ(tree.getMaxDepth(), maxDepth);
        EXPECT_EQ(tree.totalEntries(), totalEntries);
    }
};

TEST_F(Structures_Octree, DefaultConstruction)
{
    TestOctreeType tree{};
    checkTree(tree, {}, {}, {}, {}, {}, {}, 0U);
    EXPECT_TRUE(tree.getChildNodes().empty());
    auto const nodes = tree.getNodes();
    EXPECT_EQ(nodes.size(), 1U);
    for (auto i = 0U; i < 4U; ++i)
    {
        auto const nodesLayer = tree.getNodes(i);
        EXPECT_EQ(nodesLayer.size(), (i == 0 ? 1U : 0U));
    }
    EXPECT_EQ(tree.closestEntryTo({}, {}, {}), nullptr);
}

TEST_F(Structures_Octree, ParameterConstruction)
{
    TestOctreeType tree{127U, 128U, 126U, 255U, 4U, 8U};
    checkTree(tree, 127U, 128U, 126U, 255U, 4U, 8U, 0U);
    EXPECT_TRUE(tree.getChildNodes().empty());
    auto const nodes = tree.getNodes();
    EXPECT_EQ(nodes.size(), 1U);
    for (auto i = 0U; i < 4U; ++i)
    {
        auto const nodesLayer = tree.getNodes(i);
        EXPECT_EQ(nodesLayer.size(), (i == 0 ? 1U : 0U));
    }
}

TEST_F(Structures_Octree, AddSingleEntry)
{
    sut.addEntry(20U, 20U, 20U, 123U);
    EXPECT_EQ(sut.totalEntries(), 1U);
    EXPECT_TRUE(sut.getChildNodes().empty());
    auto const nodes = sut.getNodes();
    EXPECT_EQ(nodes.size(), 1U);
    for (auto i = 0U; i < 4U; ++i)
    {
        auto const nodesLayer = sut.getNodes(i);
        EXPECT_EQ(nodesLayer.size(), (i == 0 ? 1U : 0U));
    }
}

TEST_F(Structures_Octree, NodeIsSplitCorrectly)
{
    for (auto i = 0U; i < 4U; ++i)
    {
        sut.addEntry(40U * i, 40U * i, 40U * i, 123U);
    }
    auto nodes = sut.getNodes();
    EXPECT_EQ(nodes.size(), 1U);
    sut.addEntry(160U, 160U, 160U, 123U);
    nodes = sut.getNodes();
    EXPECT_EQ(nodes.size(), 9U);

    // check other nodes
    checkTree(*nodes[0], 128U, 128U, 128U, 256U, 4U, 8U, 5U);
    checkTree(*nodes[1], 64U, 64U, 64U, 128U, 4U, 7U, 4U);
    checkTree(*nodes[2], 192U, 64U, 64U, 128U, 4U, 7U, 0U);
    checkTree(*nodes[3], 64U, 192U, 64U, 128U, 4U, 7U, 0U);
    checkTree(*nodes[4], 192U, 192U, 64U, 128U, 4U, 7U, 0U);
    checkTree(*nodes[5], 64U, 64U, 192U, 128U, 4U, 7U, 0U);
    checkTree(*nodes[6], 192U, 64U, 192U, 128U, 4U, 7U, 0U);
    checkTree(*nodes[7], 64U, 192U, 192U, 128U, 4U, 7U, 0U);
    checkTree(*nodes[8], 192U, 192U, 192U, 128U, 4U, 7U, 1U);
}

TEST_F(Structures_Octree, Reset)
{
    {
        TestOctreeType tree{};
        tree.addEntry(20U, 20U, 20U, 20U);
        tree.reset(12U, 13U, 14U, 24U, 4U, 3U);
        checkTree(tree, 12U, 13U, 14U, 24U, 4U, 3U, 0U);
    }
    {
        TestOctreeType tree{12U, 13U, 14U, 24U, 4U, 3U};
        tree.addEntry(20U, 20U, 20U, 20U);
        tree.reset(14U, 11U, 14U, 48U, 8U, 7U);
        checkTree(tree, 14U, 11U, 14U, 48U, 8U, 7U, 0U);
    }
}

TEST_F(Structures_Octree, AnalyzeEntries)
{
    for (auto i = 20U; i < 80; ++i)
    {
        sut.addEntry(i, i, i, i);
    }

    auto count  = 0U;
    auto lambda = [&](TestOctreeType::Entry const &e) {
        EXPECT_EQ(e.x, e.y);
        EXPECT_EQ(e.x, e.z);
        EXPECT_EQ(e.x, e.value);
        ++count;
    };
    sut.analyzeEntries(lambda);
    EXPECT_EQ(count, sut.totalEntries());
}

TEST_F(Structures_Octree, ClosestEntryToReturnsClosestEntryOfUnsplitNode)
{
    sut.addEntry(32U, 48U, 24U, 10);
    sut.addEntry(132U, 148U, 124U, 11);
    sut.addEntry(64U, 97U, 234U, 12);
    auto const result = sut.closestEntryTo(35U, 60U, 100U);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->value, 10U);
}

TEST_F(Structures_Octree, ClosestEntryToReturnsClosestEntryOfSplitNode)
{
    sut.addEntry(32U, 48U, 24U, 10);
    sut.addEntry(32U, 50U, 24U, 11);
    sut.addEntry(64U, 97U, 23U, 12);
    sut.addEntry(60U, 76U, 33U, 13);
    sut.addEntry(134U, 17U, 43U, 14);
    auto result = sut.closestEntryTo(35U, 60U, 40U);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->value, 11U);

    result = sut.closestEntryTo(135U, 160U, 140U);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->value, 12U);
}

TEST_F(Structures_Octree, ClosestEntryToReturnsNullptrIfCloserEntryAlreadyHasBeenFound)
{
    sut.reset(32U, 32U, 32U, 64U, 4U, 8U);
    sut.addEntry(63U, 63U, 63U, 10);
    EXPECT_EQ(sut.closestEntryTo(128U, 128U, 128U, 4096U), nullptr);
}

} // namespace Terrahertz::UnitTests
