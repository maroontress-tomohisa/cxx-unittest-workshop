#include "base64/ParseError.h"
#include "base64/DecodeHelper.h"
#include "base64/Decoder.h"

base64::Decoder::Decoder(EncodingMode mode)
{
    (void)mode;
}

auto
base64::Decoder::apply(const std::string& encoded) -> std::vector<uint8_t>
{
    auto result = std::vector<uint8_t> {};
    auto& table = RFC4648_TABLE;
    auto top = encoded.data();
    auto data = top;
    auto size = encoded.size();
    while (size >= 4) {
        EncodeUnit in {data[0], data[1], data[2], data[3]};
        auto v = DecodeHelper::decodeLastUnit(table, in, data - top);
        for (auto k = (std::size_t)0; k < v.size(); ++k) {
            result.push_back(v[k]);
        }
        size -= in.size();
        data += in.size();
        if (v.size() < 3 && size > 0) {
            // パディングがあったのに、まだ文字残っとる
            throw ParseError("invalid char: trailing char", data - top);
        }
    }
    auto position = data - top;
    auto zero = ZERO_CHAR;
    if (size == 1) {
        throw ParseError("invalid char: trailing char", position);
    }
    if (size == 0) {
        return result;
    }
    auto third = (size == 2) ? zero : data[2];
    auto in = EncodeUnit {data[0], data[1], third, zero};
    auto v = DecodeHelper::decodeUnit(table, in, position);
    auto y = size - 1;
    for (auto k = 0; k < y; ++k) {
        result.push_back(v[k]);
    }
    if (v[y]) {
        throw ParseError("invalid char: canonical encoding violation", position + y);
    }
    return result;
}
