#ifndef base64_Decoder_H
#define base64_Decoder_H

#include <cstdint>
#include <vector>
#include <string>
#include "base64/EncodingMode.h"

namespace base64 {

    /** BASE64エンコーディングのデコード機能を提供します。 */
    class Decoder final {
    public:

        /**
            コンストラクタです。

            @param mode エンコーディングの種類。デフォルトは EncodingMode::BASE64
            です。
        */
        explicit Decoder(EncodingMode mode = EncodingMode::BASE64);

        /**
            BASE64エンコーディングで符号化された文字列をバイト配列に復号します。

            @a encoded がパディングを含む場合と含まない場合のどちらも正しくデコードします。

            デコード中、不正な文字を検出した場合は例外 ParseError をスローします。
            例外 ParseError の関数 @c getPosition() は不正な文字の位置を返します。
            不正な文字が複数ある場合は、最も小さい数の位置を返します。

            @param encoded 符号化された文字列。
            @return デコードされたバイト列。
        */
        auto apply(const std::string& encoded) -> std::vector<uint8_t>;

    private:
    };
}

#endif
