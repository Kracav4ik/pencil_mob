#include "gtest/gtest.h"
#include "transport.h"

TEST(transport, test_encode) {
    EXPECT_EQ(encode(0x00000000), QByteArray("\x00", 1));
    EXPECT_EQ(encode(0x00000001), QByteArray("\x01", 1));
    EXPECT_EQ(encode(0x0000007f), QByteArray("\x7f", 1));
    EXPECT_EQ(encode(0x00000080), QByteArray("\x81\x00", 2));
    EXPECT_EQ(encode(0x00003fff), QByteArray("\xff\x7f", 2));
    EXPECT_EQ(encode(0x00004000), QByteArray("\x81\x80\x00", 3));
    EXPECT_EQ(encode(0x00004001), QByteArray("\x81\x80\x01", 3));
    EXPECT_EQ(encode(0x00004083), QByteArray("\x81\x81\x03", 3));
    EXPECT_EQ(encode(0x001fffff), QByteArray("\xff\xff\x7f", 3));
    EXPECT_EQ(encode(0x00200000), QByteArray("\x81\x80\x80\x00", 4));
    EXPECT_EQ(encode(0x00200001), QByteArray("\x81\x80\x80\x01", 4));
    EXPECT_EQ(encode(0x00200083), QByteArray("\x81\x80\x81\x03", 4));
    EXPECT_EQ(encode(0x00204187), QByteArray("\x81\x81\x83\x07", 4));
    EXPECT_EQ(encode(0x0fffffff), QByteArray("\xff\xff\xff\x7f", 4));
    EXPECT_EQ(encode(0x10000000), QByteArray("\x81\x80\x80\x80\x00", 5));
    EXPECT_EQ(encode(0x10000001), QByteArray("\x81\x80\x80\x80\x01", 5));
    EXPECT_EQ(encode(0x10000083), QByteArray("\x81\x80\x80\x81\x03", 5));
    EXPECT_EQ(encode(0x10004187), QByteArray("\x81\x80\x81\x83\x07", 5));
    EXPECT_EQ(encode(0x1020c38f), QByteArray("\x81\x81\x83\x87\x0f", 5));
    EXPECT_EQ(encode(0xffffffff), QByteArray("\x8f\xff\xff\xff\x7f", 5));
}

TEST(transport, test_decode) {
    EXPECT_EQ(0x00000000, Decoder(QByteArray("\x00", 1)).number);
    EXPECT_EQ(0x00000000, Decoder(QByteArray("\x00\x8f\x0f", 3)).number);
    EXPECT_EQ(0x00000001, Decoder(QByteArray("\x01", 1)).number);
    EXPECT_EQ(0x0000007f, Decoder(QByteArray("\x7f", 1)).number);
    EXPECT_EQ(0x00000080, Decoder(QByteArray("\x81\x00", 2)).number);
    EXPECT_EQ(0x00003fff, Decoder(QByteArray("\xff\x7f", 2)).number);
    EXPECT_EQ(0x00004000, Decoder(QByteArray("\x81\x80\x00", 3)).number);
    EXPECT_EQ(0x00004001, Decoder(QByteArray("\x81\x80\x01", 3)).number);
    EXPECT_EQ(0x00004083, Decoder(QByteArray("\x81\x81\x03", 3)).number);
    EXPECT_EQ(0x001fffff, Decoder(QByteArray("\xff\xff\x7f", 3)).number);
    EXPECT_EQ(0x00200000, Decoder(QByteArray("\x81\x80\x80\x00", 4)).number);
    EXPECT_EQ(0x00200001, Decoder(QByteArray("\x81\x80\x80\x01", 4)).number);
    EXPECT_EQ(0x00200083, Decoder(QByteArray("\x81\x80\x81\x03", 4)).number);
    EXPECT_EQ(0x00204187, Decoder(QByteArray("\x81\x81\x83\x07", 4)).number);
    EXPECT_EQ(0x0fffffff, Decoder(QByteArray("\xff\xff\xff\x7f", 4)).number);
    EXPECT_EQ(0x10000000, Decoder(QByteArray("\x81\x80\x80\x80\x00", 5)).number);
    EXPECT_EQ(0x10000001, Decoder(QByteArray("\x81\x80\x80\x80\x01", 5)).number);
    EXPECT_EQ(0x10000083, Decoder(QByteArray("\x81\x80\x80\x81\x03", 5)).number);
    EXPECT_EQ(0x10004187, Decoder(QByteArray("\x81\x80\x81\x83\x07", 5)).number);
    EXPECT_EQ(0x1020c38f, Decoder(QByteArray("\x81\x81\x83\x87\x0f", 5)).number);
    EXPECT_EQ(0xffffffff, Decoder(QByteArray("\x8f\xff\xff\xff\x7f", 5)).number);
}
