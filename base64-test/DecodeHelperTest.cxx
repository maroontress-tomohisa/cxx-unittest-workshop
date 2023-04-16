#include <string>
#include <gtest/gtest.h>
#include "base64/namespace.h"
#include "base64/DecodeHelper.h"
#include "base64/ParseError.h"

using base64::EncodeUnit;
using base64::DecodeUnit;
using base64::DecodeHelper;
using base64::ParseError;
using base64::RFC4648_TABLE;

TEST(DecodeHelperTest, convertUnit_cafe00)
{
    auto sixBitsArray = std::array<std::uint8_t, 4> {0x32, 0x2f, 0x38, 0x00};
    //    0x32      0x2f    0x38      0x00
    //  110010    101111  111000    000000
    //  |  | \\  // |  |  |  | \\  // |  |
    //  1100  1010  1111  1110  0000  0000
    //     c     a     f     e     0     0
    auto expected = DecodeUnit {0xca, 0xfe, 0x00};
    auto actual = DecodeHelper::convertUnit(sixBitsArray);
    ASSERT_EQ(actual, expected);
}

TEST(DecodeHelperTest, decodeUnit_cafe00)
{
    auto in = EncodeUnit {'y', 'v', '4', 'A'};
    auto expected = DecodeUnit {0xca, 0xfe, 0x00};
    auto actual = DecodeHelper::decodeUnit(RFC4648_TABLE, in, 0);
    ASSERT_EQ(actual, expected);
}

TEST(DecodeHelperTest, decodeUnit_a2bcff)
{
    auto in = EncodeUnit {'o', 'r', 'z', '/'};
    auto expected = DecodeUnit {0xa2, 0xbc, 0xff};
    auto actual = DecodeHelper::decodeUnit(RFC4648_TABLE, in, 0);
    ASSERT_EQ(actual, expected);
}

static void
invalidTest(const EncodeUnit in, std::size_t offset,
    std::size_t expectedPosition)
{
    ASSERT_THROW({
            (void)DecodeHelper::decodeUnit(RFC4648_TABLE, in, offset);
        },
        ParseError);

    try {
        (void)DecodeHelper::decodeUnit(RFC4648_TABLE, in, offset);
        FAIL() << "NEVER REACHED";
    } catch (ParseError& e) {
        auto actualPosition = e.getPosition();
        ASSERT_EQ(actualPosition, expectedPosition)
            << "position mismatched";
    }
}

TEST(DecodeHelperTest, decodeUnit_nonAsciiChars)
{
    auto in = EncodeUnit {'\x80', '\x81', '\x82', '\x83'};
    const auto offset = (std::size_t)10;
    invalidTest(in, offset, offset);
}

TEST(DecodeHelperTest, decodeUnit_illegalChar1)
{
    auto in = EncodeUnit {'!', 'v', '4', 'A'};
    const auto offset = (std::size_t)10;
    invalidTest(in, offset, offset);
}

TEST(DecodeHelperTest, decodeUnit_illegalChar2)
{
    auto in = EncodeUnit {'y', '!', '4', 'A'};
    const auto offset = (std::size_t)10;
    invalidTest(in, offset, offset + 1);
}

TEST(DecodeHelperTest, decodeUnit_illegalChar3)
{
    auto in = EncodeUnit {'y', 'v', '!', 'A'};
    const auto offset = (std::size_t)10;
    invalidTest(in, offset, offset + 2);
}

TEST(DecodeHelperTest, decodeUnit_illegalChar4)
{
    auto in = EncodeUnit {'y', 'v', '4', '!'};
    const auto offset = (std::size_t)10;
    invalidTest(in, offset, offset + 3);
}

TEST(DecodeHelperTest, decodeUnit_allIllegalChars)
{
    auto in = EncodeUnit {'!', '!', '!', '!'};
    const auto offset = (std::size_t)10;
    invalidTest(in, offset, offset);
}

TEST(DecodeHelperTest, decodeUnit_last3IllegalChars)
{
    auto in = EncodeUnit {'y', '!', '!', '!'};
    const auto offset = (std::size_t)10;
    invalidTest(in, offset, offset + 1);
}

TEST(DecodeHelperTest, decodeUnit_last2IllegalChars)
{
    auto in = EncodeUnit {'y', 'v', '!', '!'};
    const auto offset = (std::size_t)10;
    invalidTest(in, offset, offset + 2);
}
