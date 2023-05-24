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

auto base64::DecodeHelper::decodeLastUnit(const DecodeTable &table,
                                          const EncodeUnit &in,
                                          std::size_t position)
    -> std::vector<std::uint8_t>
{
    std::array<uint8_t, 4> sixBitsArray{};
    auto x = (std::size_t)0;
    for (; x < in.size(); ++x)
    {
        auto v = (std::size_t)in[x];
        if (v >= table.size())
        {
            throw ParseError("invalid char", position + x);
        }
        auto n = table[v];
        if (n >= CODES)
        {
            break;
        }
        sixBitsArray[x] = n;
    }
    if (x < 2)
    {
        throw ParseError("invalid char", position + x);
    }
    for (auto k = x; k < in.size(); ++k)
    {
        if (in[k] != PAD_CHAR)
        {
            throw ParseError("invalid char: trailing char",
                             position + k);
        }
        sixBitsArray[k] = 0;
    }
    auto v = convertUnit(sixBitsArray);
    auto result = std::vector<std::uint8_t> {};
    auto y = x - 1;
    result.reserve(y);
    for (auto k = (std::size_t)0; k < y; ++k)
    {
        result.push_back(v[k]);
    }
    if (x < 4 && v[y] != 0) {
        // パディングがあって、かつ端数の要素が非0
        throw ParseError("invalid char: canonical encoding violation", position + y);
    }
    return result;
}
