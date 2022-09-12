#ifndef THZ_COMMON_MEMORY_IMEMORYWRAPPER_H
#define THZ_COMMON_MEMORY_IMEMORYWRAPPER_H

namespace Terrahertz {

/// @brief Interface for all classes wrapping a memory buffer.
///
/// @tparam TDataType The type of data the memory of the buffer uses.
template <typename TDataType>
class IMemoryWrapper
{
public:
    /// @brief Shortcut to the data type of the wrapper.
    using value_type = TDataType;

    /// @brief The type of pointer to the data.
    using pointer = TDataType *;

    /// @brief The type of const pointer to the data.
    using const_pointer = const TDataType *;

    /// @brief Reference to a pixel of this image.
    using reference = TDataType &;

    /// @brief Reference to a const pixel of this image.
    using const_reference = const TDataType &;

    /// @brief The size type of the image.
    using size_type = size_t;

    /// @brief Default the destructor to make it virtual.
    virtual ~IMemoryWrapper() noexcept {}

    /// @brief Returns the pointer to the first element in the wrapped buffer.
    ///
    /// @return The pointer to the first element in the wrapped buffer.
    virtual pointer data() noexcept = 0;

    /// @brief Returns the pointer to the first element in the wrapped buffer.
    ///
    /// @return The pointer to the first element in the wrapped buffer.
    virtual const_pointer data() const noexcept = 0;

    /// @brief Returns the current size of the wrapped buffer.
    ///
    /// @return The current size of the wrapped buffer.
    virtual size_t size() const noexcept = 0;

    /// @brief Resizes the wrapped buffer if possible.
    ///
    /// @param newSize The new size of the wrapped buffer.
    /// @return True if the buffer could be resized, false otherwise.
    virtual bool resize(size_t newSize) noexcept = 0;

    /// @brief Returns the maximum size the wrapped buffer can have.
    ///
    /// @return The maximum size the wrapped buffer can have.
    virtual size_t maxSize() const noexcept = 0;

    /// @brief Index operator of the image for retrieving pixels by index.
    ///
    /// @param index The index of the pixel to return.
    /// @return  The pixel at the given index.
    inline reference operator[](size_type index) noexcept { return *(data() + index); }

    /// @brief Index operator of the image for retrieving pixels by index.
    ///
    /// @param index The index of the pixel to return.
    /// @return  The pixel at the given index.
    inline const_reference operator[](size_type index) const noexcept { return *(data() + index); }
};

} // namespace Terrahertz

#endif // !THZ_COMMON_MEMORY_IMEMORYWRAPPER_H
