#ifndef THZ_COMMON_MEMORY_IMEMORYPOOL_HPP
#define THZ_COMMON_MEMORY_IMEMORYPOOL_HPP

#include <cstddef>

namespace Terrahertz {

/// @brief Interface for all memory pools.
class IMemoryPool
{
public:
    /// @brief Allocates a certain amount of bytes from the MemoryPool.
    ///
    /// @param n The number of bytes to allocate.
    /// @return Pointer to the first byte of the allocated memory block.
    /// @exception bad_alloc In case the memory could not be allocated.
    virtual char *allocate(size_t n) noexcept(false) = 0;

    /// @brief Deallocates the given memory block.
    ///
    /// @param p The pointer to the first byte in the block.
    /// @param n The size of the block [bytes].
    virtual void deallocate(char *p, size_t n) noexcept = 0;

    /// @brief Returns the amount of used space in the MemoryPool [bytes].
    ///
    /// @return The amount of used space in the MemoryPool [bytes].
    virtual size_t usedSpace() const noexcept = 0;

    /// @brief Returns the total amount of space in the MemoryPool [bytes].
    ///
    /// @return The total amount of space in the MemoryPool [bytes].
    virtual size_t totalSpace() const noexcept = 0;

    /// @brief Returns the level of the memory pool [%].
    ///
    /// @return The level of the memory pool [%].
    double level() const noexcept { return (100.0 * usedSpace()) / totalSpace(); }
};

} // namespace Terrahertz

#endif // !THZ_COMMON_MEMORY_IMEMORYPOOL_HPP
