#ifndef THZ_COMMON_UTILITY_STATICPIMPL_H
#define THZ_COMMON_UTILITY_STATICPIMPL_H

#include <array>
#include <cstdint>

namespace Terrahertz {

/// @brief Helper class for using static memory for the pointer-to-implementation-idiom.
///
/// @tparam TImplType The type of the implementation.
/// @tparam TImplSize The size of the implementation.
template <typename TImplType, size_t TImplSize>
class StaticPImpl
{
public:
    /// @brief Pointer to the implementation.
    using pointer = TImplType *;

    /// @brief Reference to the implementation.
    using reference = TImplType &;

    /// @brief Default initializes the wrapper.
    StaticPImpl() noexcept = default;

    /// @brief Prevent copy construction by deleting the constructor.
    StaticPImpl(StaticPImpl const &other) noexcept = delete;

    /// @brief Prevent move construction by deleting the constructor.
    StaticPImpl(StaticPImpl &&other) noexcept = delete;

    /// @brief Prevent copy assignment by deleting the operator.
    StaticPImpl &operator=(StaticPImpl const &other) noexcept = delete;

    /// @brief Prevent move assignment by deleting the operator.
    StaticPImpl &operator=(StaticPImpl &&other) noexcept = delete;

    /// @brief Finalizes the implementation, calling the destructor.
    ~StaticPImpl() noexcept { _pointer->~TImplType(); }

    /// @brief Initializes the Implementation forwarding the given parameters to its constructor.
    ///
    /// @tparam ...TParamTypes The type of the parameters.
    /// @param ...params The parameters.
    template <typename... TParamTypes>
    inline void init(TParamTypes &&...params) noexcept
    {
        static_assert(sizeof(TImplType) == TImplSize, "TImplSize too small for TImplType");
        _pointer = new (_buffer.data()) TImplType(std::forward<TParamTypes>(params)...);
    }

    /// @brief Returns a pointer to the implementation.
    ///
    /// @return A pointer to the implementation.
    [[nodiscard]] pointer operator->() const noexcept { return _pointer; }

    /// @brief Returns a reference to the implementation.
    ///
    /// @return A reference to the implementation.
    [[nodiscard]] reference operator*() const noexcept { return *_pointer; }

private:
    /// @brief The pointer to the implementation.
    TImplType *_pointer{};

    /// @brief The buffer to store the instance.
    std::array<std::uint8_t, TImplSize> _buffer{};
};

} // namespace Terrahertz

#endif // !THZ_COMMON_UTILITY_STATICPIMPL_H
