#ifndef base64_ParseError_H
#define base64_ParseError_H

#include <cstdint>
#include <string>
#include <stdexcept>

namespace base64 {

    /** BASE64エンコーディングのデコード中のパース失敗を表す例外です。 */
    class ParseError final : public std::runtime_error {
    public:
        /**
            コンストラクタです。

            @param message メッセージ。
            @param position エラーが発生した場所（先頭の文字を0とする）。
        */
        explicit ParseError(const std::string& message, std::size_t position)
            : std::runtime_error(message), position(position) {
        }

        /**
            コンストラクタです。

            @param message メッセージ。
            @param position エラーが発生した場所（先頭の文字を0とする）。
        */
        explicit ParseError(const char* message, std::size_t position)
            : std::runtime_error(message), position(position) {
        }

        /**
            エラーが発生した場所を返します。

            @return エラーが発生した場所（先頭の文字を0とする）。
        */
        auto getPosition() const -> std::size_t {
            return position;
        }

    private:
        std::size_t position;
    };
}

#endif
