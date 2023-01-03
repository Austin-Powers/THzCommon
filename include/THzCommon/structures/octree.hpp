#ifndef THZ_COMMON_STRUCTURES_OCTREE_HPP
#define THZ_COMMON_STRUCTURES_OCTREE_HPP

#include <cstdint>
#include <gsl/span>
#include <type_traits>
#include <vector>

namespace Terrahertz {

/// @brief Implementation of an octree based on a cube shaped space.
///
/// @tparam TCoordinateType The type used for the coordinates inside the octree space.
/// @tparam TSizeType The type used to measure the octree space (needs to be bigger than TCoordinateType).
/// @tparam TDataType The data type of the elements inside the octree.
template <typename TCoordinateType, typename TSizeType, typename TDataType>
class Octree
{
public:
    static_assert(std::is_arithmetic_v<TCoordinateType>, "TCoordinateType needs to be arithmetic");
    static_assert(sizeof(TCoordinateType) <= sizeof(TSizeType),
                  "TCoordinateType needs to be smaller or equal to TSizeType");

    /// @brief Represents an entry in the octree.
    struct Entry
    {
        /// @brief The x coordinate of the entry.
        TCoordinateType x{};

        /// @brief The y coordinate of the entry.
        TCoordinateType y{};

        /// @brief The z coordinate of the entry.
        TCoordinateType z{};

        /// @brief The value of the entry.
        TDataType value{};
    };

    /// @brief Shortcut to this type.
    using MyType = Octree<TCoordinateType, TSizeType, TDataType>;

    /// @brief A vector of pointers to constant nodes on a Octree.
    using CNodePointerVector = std::vector<MyType const *>;

    /// @brief Default initializes a new Octree.
    Octree() noexcept = default;

    /// @brief Initializes a new Octree at the given position.
    ///
    /// @param centerX The geometric center of the node on the x-axis.
    /// @param centerY The geometric center of the node on the y-axis.
    /// @param centerZ The geometric center of the node on the z-axis.
    /// @param size The size of the node in all directions.
    /// @param maxEntries The maximum number of entries in a node before it gets subdivided.
    /// @param maxDepth The maximum depth of the tree.
    Octree(TCoordinateType centerX,
           TCoordinateType centerY,
           TCoordinateType centerZ,
           TSizeType       size,
           uint32_t        maxEntries,
           uint8_t         maxDepth) noexcept
        : _centerX{centerX},
          _centerY{centerY},
          _centerZ{centerZ},
          _size{size},
          _maxEntries{maxEntries},
          _maxDepth{maxDepth}
    {}

    /// @brief Returns the center of the octree on the x axis.
    ///
    /// @return The center of the octree on the x axis.
    TCoordinateType getCenterX() const noexcept { return _centerX; }

    /// @brief Returns the center of the octree on the y axis.
    ///
    /// @return The center of the octree on the y axis.
    TCoordinateType getCenterY() const noexcept { return _centerY; }

    /// @brief Returns the center of the octree on the z axis.
    ///
    /// @return The center of the octree on the z axis.
    TCoordinateType getCenterZ() const noexcept { return _centerZ; }

    /// @brief Returns the size of the octree.
    ///
    /// @return The size of the octree.
    TSizeType getSize() const noexcept { return _size; }

    /// @brief Returns the maximum number of entries in a node before it gets subdivided.
    ///
    /// @return The maximum number of entries in a node before it gets subdivided.
    uint32_t getMaxEntries() const noexcept { return _maxEntries; }

    /// @brief Return the maximum depth of the tree.
    ///
    /// @return The maximum depth of the tree.
    uint8_t getMaxDepth() const noexcept { return _maxDepth; }

    /// @brief Adds a new entry to the octree.
    ///
    /// @param e The entry to add.
    void addEntry(Entry const &e) noexcept
    {
        _touched = true;
        if (!_children.empty())
        {
            _children[positionToIndex(e.x, e.y, e.z)].addEntry(e);
        }
        else
        {
            if ((_entries.size() < _maxEntries) || (_maxDepth == 0))
            {
                _entries.push_back(e);
            }
            else
            {
                // split node
                auto const childSize   = _size / 2;
                auto const childOffset = childSize / 2;

                auto const xp = _centerX + childOffset;
                auto const xn = _centerX - childOffset;
                auto const yp = _centerY + childOffset;
                auto const yn = _centerY - childOffset;
                auto const zp = _centerZ + childOffset;
                auto const zn = _centerZ - childOffset;
                _children.emplace_back(xn, yn, zn, childSize, _maxEntries, _maxDepth - 1);
                _children.emplace_back(xp, yn, zn, childSize, _maxEntries, _maxDepth - 1);
                _children.emplace_back(xn, yp, zn, childSize, _maxEntries, _maxDepth - 1);
                _children.emplace_back(xp, yp, zn, childSize, _maxEntries, _maxDepth - 1);
                _children.emplace_back(xn, yn, zp, childSize, _maxEntries, _maxDepth - 1);
                _children.emplace_back(xp, yn, zp, childSize, _maxEntries, _maxDepth - 1);
                _children.emplace_back(xn, yp, zp, childSize, _maxEntries, _maxDepth - 1);
                _children.emplace_back(xp, yp, zp, childSize, _maxEntries, _maxDepth - 1);

                addEntry(e);
                for (auto const &entry : _entries)
                {
                    addEntry(entry);
                }
                _entries.clear();
            }
        }
    }

    /// @brief Initializes a new Octree at the given position.
    ///
    /// @param x The x coordinate of the entry.
    /// @param y The y coordinate of the entry.
    /// @param z The z coordinate of the entry.
    /// @param value The value coordinate of the entry.
    void addEntry(TCoordinateType x, TCoordinateType y, TCoordinateType z, TDataType value) noexcept
    {
        addEntry(Entry{x, y, z, value});
    }

    /// @brief Resets this node, removing all children and entries and setting the given values.
    ///
    /// @param centerX The geometric center of the node on the x-axis.
    /// @param centerY The geometric center of the node on the y-axis.
    /// @param centerZ The geometric center of the node on the z-axis.
    /// @param size The size of the node in all directions.
    /// @param maxEntries The maximum number of entries in a node before it gets subdivided.
    /// @param maxDepth The maximum depth of the tree.
    void reset(TCoordinateType centerX,
               TCoordinateType centerY,
               TCoordinateType centerZ,
               TSizeType       size,
               uint32_t        maxEntries,
               uint8_t         maxDepth) noexcept
    {
        _centerX    = centerX;
        _centerY    = centerY;
        _centerZ    = centerZ;
        _size       = size;
        _maxEntries = maxEntries;
        _maxDepth   = maxDepth;
        reset();
    }

    /// @brief Resets this node, removing all children and entries.
    void reset() noexcept
    {
        if (!_children.empty())
        {
            for (auto &child : _children)
            {
                child.reset();
            }
            _children.clear();
        }
        _entries.clear();
    }

    /// @brief Returns the children of this node, or an empty span if this nodes has no children.
    ///
    /// @return The children of this node, or an empty span if this nodes has no children.
    gsl::span<Octree const> getChildNodes() const noexcept { return {_children}; }

    /// @brief Returns pointers to all nodes inside the Octree.
    ///
    /// @param nodes Output: A vector to store the pointers to all nodes inside the Octree.
    void getNodes(std::vector<MyType const *> &nodes) const noexcept
    {
        nodes.push_back(this);
        for (auto const &child : _children)
        {
            child.getNodes(nodes);
        }
    }

    /// @brief Returns pointers to all nodes inside the Octree.
    ///
    /// @param nodes Output: A vector to store the pointers to all nodes inside the Octree.
    /// @param layer The layer from which the nodes shall be retrieved, 0 for this layer, 1 for child layer and so on.
    void getNodes(std::vector<MyType const *> &nodes, size_t const layer) const noexcept
    {
        if (layer == 0)
        {
            nodes.push_back(this);
        }
        else
        {
            for (auto const &child : _children)
            {
                child.getNodes(nodes, layer - 1);
            }
        }
    }

    /// @brief Returns a vector of pointers to all nodes inside the Octree.
    ///
    /// @return A vector of pointers to all nodes inside the Octree.
    CNodePointerVector getNodes() const noexcept
    {
        CNodePointerVector nodes{};
        getNodes(nodes);
        return nodes;
    }

    /// @brief Returns a vector of pointers to all nodes inside the Octree.
    ///
    /// @param layer The layer from which the nodes shall be retrieved, 0 for this layer, 1 for child layer and so on.
    /// @return A vector of pointers to all nodes inside the Octree.
    CNodePointerVector getNodes(size_t const layer) const noexcept
    {
        CNodePointerVector nodes{};
        getNodes(nodes, layer);
        return nodes;
    }

    /// @brief Uses the given function to analyze the entries of this node and its children.
    ///
    /// @tparam TFunction The type of the function to analyze the entries.
    /// @param function The function to analyze the entries.
    template <typename TFunction>
    void analyzeEntries(TFunction &function) const noexcept
    {
        for (auto const &entry : _entries)
        {
            function(entry);
        }
        for (auto const &child : _children)
        {
            child.analyzeEntries(function);
        }
    }

    /// @brief Recalculates all meta data for faster processing.
    void recalculate() noexcept
    {
        if (_touched)
        {
            _totalEntries = _entries.size();
            for (auto &child : _children)
            {
                child.recalculate();
                _totalEntries += child.totalEntries();
            }
            _touched = false;
        }
    }

    /// @brief Returns the number of entries this node or its children have in total.
    ///
    /// @return The number of entries this node or its children have in total.
    size_t totalEntries() noexcept
    {
        recalculate();
        return _totalEntries;
    }

    /// @brief Returns the number of entries this node or its children have in total.
    ///
    /// @return The number of entries this node or its children have in total.
    size_t totalEntries() const noexcept
    {
        if (_touched)
        {
            auto totalEntries = _entries.size();
            for (auto &child : _children)
            {
                totalEntries += child.totalEntries();
            }
            return totalEntries;
        }
        return _totalEntries;
    }

private:
    /// @brief Turns the given position into an index in the _children vector.
    ///
    /// @param x The position on the X-axis.
    /// @param y The position on the Y-axis.
    /// @param z The position on the Z-axis.
    /// @return The index corresponding to the position.
    std::uint_fast8_t positionToIndex(TCoordinateType x, TCoordinateType y, TCoordinateType z) const noexcept
    {
        std::uint_fast8_t index = 0U;
        if (x >= _centerX)
        {
            index |= 0x1;
        }
        if (y >= _centerY)
        {
            index |= 0x2;
        }
        if (z >= _centerZ)
        {
            index |= 0x4;
        }
        return index;
    }

    /// @brief The geometric center of the node on the x-axis.
    TCoordinateType _centerX{};

    /// @brief The geometric center of the node on the y-axis.
    TCoordinateType _centerY{};

    /// @brief The geometric center of the node on the z-axis.
    TCoordinateType _centerZ{};

    /// @brief The size of the node in all directions.
    TSizeType _size{};

    /// @brief The maximum number of entries in a node before it gets subdivided.
    uint32_t _maxEntries{};

    /// @brief The maximum depth of the tree.
    uint8_t _maxDepth{};

    /// @brief The child nodes of this node.
    std::vector<Octree> _children{};

    /// @brief The entries of this node.
    std::vector<Entry> _entries{};

    /// @brief Flag signalling if the node has been changed.
    bool _touched{};

    /// @brief The total number of entries in this node or its children.
    size_t _totalEntries{};
};

} // namespace Terrahertz

#endif // !THZ_COMMON_STRUCTURES_OCTREE_HPP
