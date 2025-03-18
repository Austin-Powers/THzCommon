#ifndef THZ_COMMON_RANDOM_ANT_HPP
#define THZ_COMMON_RANDOM_ANT_HPP

#include <array>
#include <cstdint>
#include <gsl/span>

namespace Terrahertz {

/// @brief Uses Langtons-Ant as a source of random numbers.
class Ant final
{
public:
    /// @brief The length of the grid the ant is working on.
    static constexpr uint64_t GRIDLENGTH = 64ULL;

    /// @brief Initializes a new random number generator with a random state.
    Ant() noexcept;

    /// @brief Loads the state of the generator from the given encoded data.
    ///
    /// @param encodedData The encoded data to load.
    /// @return True if data was loaded successfully, false otherwise.
    bool load(gsl::span<char const> const encodedData) noexcept;

    /// @brief Tries to saves the state of the generator to the given buffer.
    ///
    /// @param buffer The buffer to put the encoded in.
    /// @return The part of the buffer the data was written to, empty if the buffer was not big enough.
    gsl::span<char> save(gsl::span<char> buffer) noexcept;

    /// @brief Returns the current step of the generator.
    ///
    /// @return The current step of the generator.
    std::uint32_t step() const noexcept;

    /// @brief Moves this generator to the given step of another generator.
    ///
    /// @param step The step to sync to.
    void syncTo(std::uint32_t const step) noexcept;

    /// @brief Generates the next random byte.
    ///
    /// @return The next random byte.
    std::uint8_t nextByte() noexcept;

private:
    /// @brief Moves the ant one step forward.
    ///
    /// @return True if the ant turned right, false if it turned left.
    bool forward() noexcept;

    /// @brief Moves the ant one step backward.
    void backward() noexcept;

    /// @brief Position of the ant on the x-axis.
    std::uint8_t _posX{};

    /// @brief Position of the ant on the y-axis.
    std::uint8_t _posY{};

    /// @brief The direction the ant is is pointing towards.
    std::uint8_t _direction{};

    /// @brief Lookup table for the movement on the x-axis.
    std::array<std::int8_t, 4U> _xLookup{};

    /// @brief Lookup table for the movement on the y-axis.
    std::array<std::int8_t, 4U> _yLookup{};

    /// @brief The move sequence R=0 L=1.
    std::array<std::uint8_t, 16U> _sequence{};

    /// @brief The step of the generator.
    std::uint32_t _step{};

    /// @brief The grid the ant is walking on.
    std::array<std::uint8_t, GRIDLENGTH * GRIDLENGTH> _grid{};
};

} // namespace Terrahertz

#endif // !THZ_COMMON_RANDOM_ANT_HPP
