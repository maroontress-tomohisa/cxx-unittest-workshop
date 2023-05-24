#ifndef base64_DecodeHelper_H
#define base64_DecodeHelper_H

#include <cstdint>
#include <array>
#include <vector>
#include "base64/namespace.h"

namespace base64 {

    /**
        BASE64のデコードに役立つ実装を提供します。
    */
    class DecodeHelper final {
    public:

        /**
            BASE64エンコーディング単位をデコードします。

            @a in にエラーが含まれていた場合は例外 ParseError をスローします。

            @param table デコードに用いるテーブル。 RFC4648_TABLE を指定可能です。
            @param in デコードの対象。パディングや改行文字などを含むことはできません。
            @param position デコードの対象の出現位置。エラーで例外をスローする場合、
            この値をオフセットとしてエラー発生位置を計算します。
            @return デコードされた値。
        */
        static auto decodeUnit(const DecodeTable& table, const EncodeUnit& in,
            std::size_t position) -> DecodeUnit;

        static auto decodeLastUnit(const DecodeTable &table, const EncodeUnit &in,
            std::size_t position) -> std::vector<std::uint8_t>;

        /**
            与えられた配列の4つの6ビット値を、 DecodeUnit に変換します。

            @a in の各要素の下位6ビット以外が0でない場合、結果は不定です。

            @param in 4つの6ビット値。
            @return 3つの8ビット値。
        */
        static auto convertUnit(const std::array<std::uint8_t, 4>& in)
            noexcept -> DecodeUnit;

    private:
        /** インスタンス化を禁止します。 */
        DecodeHelper() {
        }
    };
}

#endif
