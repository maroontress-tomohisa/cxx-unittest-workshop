#include <cstring>
#include <gtest/gtest.h>
#include "base64/Decoder.h"
#include "base64/ParseError.h"

static void
base64Test(const char* encoded, const char* expected)
{
    auto decoder = base64::Decoder {};
    auto actual = decoder.apply(encoded);
    auto actualSize = actual.size();
    auto expectedSize = std::strlen(expected);
    ASSERT_EQ(actualSize, expectedSize) << "size mismatched";
    auto data = actual.data();
    // actualはnull-terminatedではないのでASSERT_STREQはつかえまへん
    for (auto k = (std::size_t)0; k < expectedSize; ++k) {
        auto actualByte = data[k];
        auto expectedByte = (std::uint8_t)expected[k];
        ASSERT_EQ(actualByte, expectedByte) << "element mismatched: k=" << k;
    }
}

TEST(DecoderTest, noPad)
{
    base64Test("TWFueSBoYW5kcyBtYWtlIGxpZ2h0IHdvcmsu",
        "Many hands make light work.");
}

TEST(DecoderTest, pad1)
{
    base64Test("bGlnaHQgd29yay4=", "light work.");
}

TEST(DecoderTest, pad2)
{
    base64Test("bGlnaHQgdw==", "light w");
}

TEST(DecoderTest, pad0)
{
    base64Test("bGlnaHQg", "light ");
}

TEST(DecoderTest, trailing3charWithoutPad)
{
    base64Test("bGlnaHQgd29yay4", "light work.");
}

TEST(DecoderTest, trailing2charWithoutPad)
{
    base64Test("bGlnaHQgdw", "light w");
}

static void
invalidTest(const char* misencoded, std::size_t expectedPosition)
{
    ASSERT_THROW({
            auto decoder = base64::Decoder {};
            (void)decoder.apply(misencoded);
        },
        base64::ParseError);

    auto decoder = base64::Decoder {};
    try {
        (void)decoder.apply(misencoded);
        FAIL() << "NEVER REACHED";
    } catch (base64::ParseError& e) {
        auto actualPosition = e.getPosition();
        ASSERT_EQ(actualPosition, expectedPosition)
            << "position mismatched";
        std::cout << misencoded << "\n";
        std::string indent(actualPosition, ' ');
        std::cout << indent << "^ (" << actualPosition << ")\n";
    }
}

TEST(DecoderTest, illegalCharHyphen)
{
    invalidTest("bGlnaH-gd29yay4=", 6);
}

TEST(DecoderTest, illegalCharUnderscore)
{
    invalidTest("bGlnaHQ_d29yay4=", 7);
}

TEST(DecoderTest, illegalCharAndExtraChar)
{
    invalidTest("bGlnaHQ_d29yay4=?", 7);
}

TEST(DecoderTest, startWithNonAsciiChars)
{
    invalidTest("\xff\xff\xff\xffyv4A", 0);
}

TEST(DecoderTest, endWithNonAsciiChars)
{
    invalidTest("yv4A\xff\xff\xff\xff", 4);
}

TEST(DecoderTest, illegalPad1)
{
    invalidTest("ay4*", 3);
}

TEST(DecoderTest, illegalPad2)
{
    invalidTest("dw=*", 3);
}

TEST(DecoderTest, twoIllegalPads)
{
    invalidTest("dw**", 2);
}

TEST(DecoderTest, trailing1char)
{
    invalidTest("bGlnaHQgd", 8);
}

TEST(DecoderTest, trailing1charWith1pad)
{
    invalidTest("bGlnaHQgd=", 9);
}

TEST(DecoderTest, trailing1charWith2pad)
{
    invalidTest("bGlnaHQgd==", 9);
}

TEST(DecoderTest, trailing1charWith3pad)
{
    invalidTest("bGlnaHQgd===", 9);
}

TEST(DecoderTest, extraPad1)
{
    invalidTest("bGlnaHQg=", 8);
}

TEST(DecoderTest, extraPad2)
{
    invalidTest("bGlnaHQg==", 8);
}

TEST(DecoderTest, extraPad3)
{
    invalidTest("bGlnaHQg===", 8);
}

TEST(DecoderTest, extraPad4)
{
    invalidTest("bGlnaHQg====", 8);
}
