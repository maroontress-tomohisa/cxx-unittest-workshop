#ifndef base64_EncodingMode_H
#define base64_EncodingMode_H

namespace base64 {

    /** BASE64エンコーディングの種類を表す定数です。 */
    enum class EncodingMode {

        /** RFC4648: base64 */
        BASE64,

        /** RFC4648: base64url */
        BASE64URL,

        /** MIME */
        MIME,
    };
}

#endif
