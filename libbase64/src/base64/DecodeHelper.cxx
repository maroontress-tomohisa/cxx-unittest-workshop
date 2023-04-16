#include "base64/ParseError.h"
#include "base64/DecodeHelper.h"

auto
base64::DecodeHelper::decodeUnit(const DecodeTable& table,
    const EncodeUnit& in, std::size_t position) -> DecodeUnit
{
    auto sixBitsArray = std::array<std::uint8_t, 4> {};
    for (auto k = (std::size_t)0; k < in.size(); ++k) {
        auto v = (std::size_t)in[k];
        if (v >= table.size()) {
            throw ParseError("invalid char", position + k);
        }
        auto n = table[v];
        if (n >= CODES) {
            throw ParseError("invalid char", position + k);
        }
        sixBitsArray[k] = n;
    }
    return convertUnit(sixBitsArray);
}

auto
base64::DecodeHelper::convertUnit(const std::array<std::uint8_t, 4>& in)
    noexcept -> DecodeUnit
{
    auto value = (std::uint32_t)in[0];
    for (auto k = (std::size_t)1; k < in.size(); ++k) {
        value <<= BITS_PER_CHAR;
        value |= in[k];
    }
    return {(std::uint8_t)(value >> 16),
        (std::uint8_t)(value >> 8),
        (std::uint8_t)value};
}
