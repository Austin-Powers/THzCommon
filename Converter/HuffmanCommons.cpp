#include "HuffmanCommons.h"

#include "Logging/Logging.h"
#include "Structures/Stack.h"
#include "Utility/SpanHelpers.h"

#include <algorithm>

namespace Terrahertz::Huffman {

/// @brief Value of an invalid entry in the tree.
constexpr std::uint16_t Invalid = 0xFFFFU;

/// @brief The bit used for the missing symbol flag.
constexpr std::uint16_t MissingSymbolsFlag = 0x8000U;

/// @brief Function calculating the amount of bytes needed to store the given number of bits.
///
/// @tparam TType The type the counter is given in.
/// @param count The bit count.
/// @return The bytes needed to store the given bits.
template <typename TType>
constexpr TType bytesForBits(TType count) noexcept
{
    return (count + 7U) / 8U;
}

CodeTable::CodeTable() noexcept
{
    addProjectName<HuffmanProject>();
    reset();
}

void CodeTable::create(SymbolDistribution const &symbolDistribution) noexcept
{
    resetTree();

    for (auto i = 0U; i < _codes.size(); ++i)
    {
        _codes[i].present = (symbolDistribution[i] != 0U);
        _tree[i].amount   = symbolDistribution[i];
    }

    // create tree
    for (auto nodeIdx = 0x100U; nodeIdx < _tree.size(); ++nodeIdx)
    {
        auto &node = _tree[nodeIdx];
        for (auto idx = 0U; idx < nodeIdx; ++idx)
        {
            auto &subNode = _tree[idx];
            if ((subNode.parent != Invalid) || (subNode.amount == 0U))
            {
                continue;
            }
            if (node.children[0] == Invalid || node.children[1] == Invalid)
            {
                node.children[0] = node.children[1];
                node.children[1] = idx;
            }
            else
            {
                auto const child0Amount = _tree[node.children[0]].amount;
                auto const child1Amount = _tree[node.children[1]].amount;
                if (child1Amount > subNode.amount)
                {
                    if (child0Amount > child1Amount)
                    {
                        node.children[0] = node.children[1];
                    }
                    node.children[1] = idx;
                }
            }
        }
        if (node.children[0] < 0x200U && node.children[1] < 0x200U)
        {
            node.amount = _tree[node.children[0]].amount + _tree[node.children[1]].amount;

            _tree[node.children[0]].parent = nodeIdx;
            _tree[node.children[1]].parent = nodeIdx;
            _rootIndex                     = nodeIdx;
        }
    }
    createTableFromTree();
}

std::int16_t CodeTable::encode(BitBufferWriter &buffer, std::uint8_t symbol) const noexcept
{
    auto const &code = _codes[symbol];
    if (!code.present)
    {
        return -1;
    }
    auto const length = 1ULL + code.length;
    return static_cast<std::uint16_t>(length - buffer.write(code.span, length));
}

std::int16_t CodeTable::encode(BitBufferWriter &buffer, std::uint8_t symbol, size_t lastBits) const noexcept
{
    auto const &code = _codes[symbol];
    if (!code.present)
    {
        return -1;
    }
    auto const length = 1ULL + code.length - lastBits;
    return static_cast<std::uint16_t>(lastBits - buffer.write(code.span, lastBits, length));
}

std::uint8_t CodeTable::decode(BitBufferReader &buffer) const noexcept
{
    auto idx = _rootIndex;
    while (idx > 0xFF)
    {
        idx = buffer.next() ? _tree[idx].children[1] : _tree[idx].children[0];
    }
    return static_cast<std::uint8_t>(idx);
}

void CodeTable::reset() noexcept
{
    for (auto i = 0U; i < _codes.size(); ++i)
    {
        _codes[i].present   = true;
        _codes[i].array[0U] = i;
        _codes[i].length    = 7U;
    }
}

gsl::span<std::uint8_t> CodeTable::write(gsl::span<std::uint8_t> buffer) const noexcept
{
    // test if table fits the buffer
    size_t length{};
    write({}, length);
    if (length > static_cast<size_t>(buffer.size()))
    {
        logMessage<LogLevel::Error, HuffmanProject>("CodeTable: unable to write table, buffer too small");
        return buffer;
    }
    return write(buffer, length);
}

gsl::span<std::uint8_t const> CodeTable::read(gsl::span<std::uint8_t const> buffer) noexcept
{
    if (buffer.size() < 2)
    {
        logMessage<LogLevel::Error, HuffmanProject>("CodeTable: reading failed as buffer is too small for the length");
        return buffer;
    }

    std::uint16_t   length{};
    BitBufferReader reader{readFromSpan(buffer, length)};
    if (buffer.size() < (length & ~MissingSymbolsFlag))
    {
        logMessage<LogLevel::Error, HuffmanProject>(
            "CodeTable: reading failed as buffer is too small for the given table");
        return buffer;
    }

    // starts at 16 as we already incorporate the length
    auto bitsRead = 16U;

    auto const missingSymobls{(length & MissingSymbolsFlag) == MissingSymbolsFlag};
    for (auto &code : _codes)
    {
        if (reader.bitsLeft() == 0U)
        {
            logMessage<LogLevel::Error, HuffmanProject>("CodeTable: reading ran out of buffer");
            return buffer;
        }
        if (missingSymobls)
        {
            code.present = reader.next();
            ++bitsRead;
            if (!code.present)
            {
                continue;
            }
        }
        else
        {
            code.present = true;
        }

        auto const codeLengthLength = reader.next() ? 4U : 8U;
        ++bitsRead;

        code.length = 0U;
        for (auto i = 0U; i < codeLengthLength; ++i)
        {
            code.length >>= 1U;
            code.length |= (reader.next() ? 0x80U : 0x0U);
            ++bitsRead;
        }
        if (codeLengthLength == 4U)
        {
            code.length >>= 4U;
        }

        BitBufferWriter codeWriter{code.span};
        for (auto i = 0U; i <= code.length; ++i)
        {
            codeWriter.write(reader.next());
            ++bitsRead;
        }
    }

    // sanity check
    auto remainingBuffer = trySubspan(buffer, bytesForBits(bitsRead));
    auto bSize           = buffer.size();
    auto rSize           = remainingBuffer.size();
    auto trueLen         = (length & ~MissingSymbolsFlag);
    if (buffer.size() - remainingBuffer.size() != (length & ~MissingSymbolsFlag))
    {
        logMessage<LogLevel::Error, HuffmanProject>("CodeTable: read bytes differ from table length");
        return buffer;
    }

    if (!createTreeFromTable())
    {
        logMessage<LogLevel::Error, HuffmanProject>("CodeTable: unable to create tree from read table");
        return buffer;
    }
    return remainingBuffer;
}

size_t CodeTable::calculateExpectation(SymbolDistribution const &symbolDistribution) const noexcept
{
    size_t expectation{};
    // table
    write({}, expectation);
    expectation *= 8U;

    // data
    for (auto i = 0U; i < _codes.size(); ++i)
    {
        expectation += symbolDistribution[i] * (1ULL + _codes[i].length);
    }
    return bytesForBits(expectation);
}

gsl::span<std::uint8_t> CodeTable::write(gsl::span<std::uint8_t> const buffer, size_t &length) const noexcept
{
    bool const missingSymbols{std::find_if(_codes.cbegin(), _codes.cend(), [](Code const &c) noexcept -> bool {
                                  return !c.present;
                              }) != _codes.cend()};

    if ((!buffer.empty()) && (buffer.size() < 3))
    {
        return buffer;
    }

    auto            failure     = false;
    auto            bitsWritten = 0ULL;
    BitBufferWriter writer{trySubspan(buffer, 2U)};
    auto const      writeBit = [&](bool const bit) noexcept -> void {
        if (buffer.empty())
        {
            ++bitsWritten;
        }
        else if (writer.write(bit))
        {
            ++bitsWritten;
        }
        else
        {
            failure = true;
        }
    };

    for (auto const &code : _codes)
    {
        // structure:
        // 1      bit missing symbol flag (if missing symbols)
        // 1      bit short length flag
        // 4 or 8 bit length
        // length bit code

        if (missingSymbols)
        {
            writeBit(code.present);
            // we only need to check this if there are symbols missing
            if (!code.present)
            {
                continue;
            }
        }
        auto const shortCode = code.length < 16U;
        writeBit(shortCode);
        auto tempLength = code.length;
        for (auto bits = shortCode ? 4U : 8U; bits != 0; --bits)
        {
            writeBit((tempLength & 0x1U) == 0x1U);
            tempLength >>= 1U;
        }
        if (buffer.empty())
        {
            bitsWritten += (1ULL + code.length);
        }
        else
        {
            auto const result = writer.write(code.span, 1ULL + code.length);
            if (result == (1ULL + code.length))
            {
                bitsWritten += result;
            }
            else
            {
                failure = true;
            }
        }
    }

    if (failure)
    {
        return buffer;
    }

    length            = bytesForBits(bitsWritten) + 2U;
    auto headerLength = static_cast<std::uint16_t>(length);
    if (missingSymbols)
    {
        headerLength |= MissingSymbolsFlag;
    }

    return trySubspan(writeToSpan(buffer, headerLength), bytesForBits(bitsWritten));
}

void CodeTable::resetTree() noexcept
{
    for (auto &node : _tree)
    {
        node.amount      = 0x0U;
        node.parent      = Invalid;
        node.children[0] = Invalid;
        node.children[1] = Invalid;
    }
}

void CodeTable::createTableFromTree() noexcept
{
    Stack<std::uint8_t, 0x100U> stack{};
    for (auto i = 0U; i < _codes.size(); ++i)
    {
        auto &code = _codes[i];
        if (!code.present)
        {
            continue;
        }
        auto idx = i;
        while (idx != Invalid)
        {
            auto parentIdx = _tree[idx].parent;
            if (parentIdx == Invalid)
            {
                break;
            }
            auto const &parent = _tree[parentIdx];
            stack.push((parent.children[0] == idx) ? 0x0U : 0x1U);

            idx = parentIdx;
        }
        code.length = static_cast<std::uint8_t>(stack.filled() - 1U);
        BitBufferWriter bbw{code.span};
        while (stack.filled() != 0U)
        {
            bbw.write(stack.pop() == 0x1U);
        }
    }
}

bool CodeTable::createTreeFromTable() noexcept
{
    resetTree();

    auto const connect =
        [&](std::uint16_t const parentIdx, std::uint16_t const childIdx, std::uint32_t const side) noexcept -> bool {
        auto &actualChild = _tree[parentIdx].children[side];
        if (actualChild != Invalid && actualChild != childIdx)
        {
            logMessage<LogLevel::Error, HuffmanProject>("CodeTable: child index already used");
            return false;
        }
        auto &actualParent = _tree[childIdx].parent;
        if (actualParent != Invalid && actualParent != parentIdx)
        {
            logMessage<LogLevel::Error, HuffmanProject>("CodeTable: parent index already used");
            return false;
        }
        actualChild  = childIdx;
        actualParent = parentIdx;
        return true;
    };

    _rootIndex    = static_cast<std::uint16_t>(_tree.size() - 1U);
    auto nextSlot = _rootIndex - 1U;
    for (std::uint16_t symbol = 0U; symbol < 0x100U; ++symbol)
    {
        auto const &code = _codes[symbol];
        if (!code.present)
        {
            continue;
        }

        auto       treeIndex = _rootIndex;
        auto const navigate  = [&](std::uint32_t const bit) noexcept -> bool {
            auto const &node = _tree[treeIndex];
            if (node.children[bit] != Invalid)
            {
                // already has child
                treeIndex = node.children[bit];
                // child is in the symbol range => tree corrupt
                return treeIndex > 0xFFU;
            }
            else
            {
                // new connection needs to be made
                if (nextSlot > 0xFFU)
                {
                    if (!connect(treeIndex, nextSlot, bit))
                    {
                        logMessage<LogLevel::Error, HuffmanProject>("CodeTable: connecting nodes index failed");
                        return false;
                    }
                    --nextSlot;
                    treeIndex = node.children[bit];
                    return true;
                }
                // nextSlot would be in the symbol range => tree corrupt
                logMessage<LogLevel::Error, HuffmanProject>("CodeTable: exhausted number of nodes");
                return false;
            }
        };

        BitBufferReader reader{code.span};
        for (auto i = 0U; i < code.length; ++i)
        {
            if (!navigate(reader.next() ? 0x1U : 0x0U))
            {
                logMessage<LogLevel::Error, HuffmanProject>("CodeTable: navigating tree failed");
                return false;
            }
        }

        // make connection to symbol index
        if (!connect(treeIndex, symbol, reader.next() ? 0x1U : 0x0U))
        {
            logMessage<LogLevel::Error, HuffmanProject>("CodeTable: connecting symbol index failed");
            return false;
        }
    }
    return true;
}

} // namespace Terrahertz::Huffman
