#ifndef THZ_COMMON_UTILITY_STRINGVIEWHELPERS_HPP
#define THZ_COMMON_UTILITY_STRINGVIEWHELPERS_HPP

#include <string_view>

namespace Terrahertz {

/// @brief Enables easy tokenizing of string_view instances.
class StringViewTokenizer
{
public:
    /// @brief The iterator used by the tokenizer.
    class Iterator
    {
    public:
        /// @brief Default initializes the
        Iterator() noexcept = default;

        /// @brief Initializes a new Iterator instance.
        ///
        /// @param view The string_view to tokenize.
        /// @param delimiter The delimiter to tokenize the string_view by.
        Iterator(std::string_view const &view, char const delimiter) noexcept;

        /// @brief Returns the token.
        ///
        /// @return The token.
        [[nodiscard]] std::string_view operator*() const noexcept;

        /// @brief Increments the iterator.
        ///
        /// @return Reference to this iterator.
        Iterator &operator++() noexcept;

        /// @brief Increments the iterator.
        ///
        /// @return Copy of this iterator before being incremented.
        Iterator operator++(int) noexcept;

        /// @brief Compares this iterator to another iterator.
        ///
        /// @param other The other iterator to compare this one with.
        /// @return True if the this iterators index equals the other ones, false otherwise.
        [[nodiscard]] bool operator==(Iterator const &other) const noexcept;

    private:
        /// @brief The view to tokenize.
        std::string_view _view{};

        /// @brief The delimiter to tokenize the string_view by.
        char _delimiter{};
    };

    /// @brief Initializes a new tokenizer.
    ///
    /// @param view The view to tokenize.
    /// @param delimiter The delimiter to tokenize the string_view by.
    StringViewTokenizer(std::string_view const &view, char const delimiter) noexcept;

    /// @brief Returns an iterator to start the tokenizing process.
    ///
    /// @return An iterator to start the process.
    [[nodiscard]] Iterator begin() const noexcept;

    /// @brief Returns an iterator to the end of the process.
    ///
    /// @return An iterator to the end of the process.
    [[nodiscard]] Iterator end() const noexcept;

private:
    /// @brief The view to tokenize.
    std::string_view _view{};

    /// @brief The delimiter to tokenize the string_view by.
    char _delimiter{};
};

/// @brief Splits the given string view at the first occurence of the given delimiter.
///
/// @param toSplit The view to split.
/// @param delimiter The delimiter to split the view at.
/// @return Pair of first and second part of the split.
/// @remark The delimiter will not be part of either result view.
std::pair<std::string_view, std::string_view> split(std::string_view const toSplit, char const delimiter) noexcept;

/// @brief Trims whitespaces of the beginning and end of the view.
///
/// @param view The view to trim.
/// @return The trimmed view.
std::string_view trim(std::string_view view) noexcept;

} // namespace Terrahertz

#endif // !THZ_COMMON_UTILITY_STRINGVIEWHELPERS_HPP
