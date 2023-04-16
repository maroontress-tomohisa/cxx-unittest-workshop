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
    // 埋める
    //
    // ヒント: DecodeHelperのテストケースを確認してみよう
    return result;
}
