#ifndef base64_namespace_H
#define base64_namespace_H

#include <cstdint>
#include <array>

namespace base64 {

    /** BASE64エンコード用テーブルです。 */
    static constexpr const char RFC4648_CODES[]
        = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    /**
        BASE64エンコーディングに使う符号の種類※です。

        ※パディングや改行は除く
    */
    static constexpr std::size_t CODES = 64;

    /** エンコーディング単位（24ビット）のエンコード文字数です。 */
    static constexpr std::size_t ENCODE_CHARS = 4;

    /** エンコーディング単位のデコードバイト数です。 */
    static constexpr std::size_t DECODE_BYTES = 3;

    /**
        BASE64エンコードされた文字列の一文字あたりのビット数※です。

        ※パディングや改行などを除く

        DECODE_BYTES * 8 / ENCODE_CHARS と等しくなります。
    */
    static constexpr std::size_t BITS_PER_CHAR = 6;
    static_assert(DECODE_BYTES * 8 == BITS_PER_CHAR * ENCODE_CHARS);

    /** デコード用テーブルのサイズです。 */
    static constexpr std::size_t DECODE_TABLE_SIZE = 128;

    /** BASE64エンコーディングで0を表す符号です。 */
    static constexpr char ZERO_CHAR = 'A';

    /** BASE64エンコーディングのパディング文字です。 */
    static constexpr char PAD_CHAR = '=';

    /** デコード用テーブル（型名のエイリアス）です。 */
    using DecodeTable = std::array<std::uint8_t, DECODE_TABLE_SIZE>;

    /** エンコーディング単位のエンコードされた文字列（型名のエイリアス）です。 */
    using EncodeUnit = std::array<char, ENCODE_CHARS>;

    /** エンコーディング単位のデコードされたバイト列（型名のエイリアス）です。 */
    using DecodeUnit = std::array<std::uint8_t, DECODE_BYTES>;

    /**
        エンコード用テーブルの文字列からデコード用のテーブルを生成します。

        @param s エンコード用テーブル。
        @return デコード用テーブル。
    */
    static constexpr auto makeDecodeTable(const std::string& s) -> DecodeTable
    {
        auto size = s.size();
        if (size != CODES) {
            throw std::invalid_argument("s");
        }
        DecodeTable table {};
        table.fill(size);
        for (auto k = (std::size_t)0; k < size; ++k) {
            table[s[k]] = k;
        }
        return table;
    }

    /** RFC4648のBASE64エンコード向けデコードテーブルです。 */
    static constexpr const DecodeTable RFC4648_TABLE
        = makeDecodeTable(RFC4648_CODES);
}

#endif
