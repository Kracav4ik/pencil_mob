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
    EXPECT_EQ(0x00000000, decode(QByteArray("\x00", 1)));
    EXPECT_EQ(0x00000000, decode(QByteArray("\x00\x8f\x0f", 3)));
    EXPECT_EQ(0x00000001, decode(QByteArray("\x01", 1)));
    EXPECT_EQ(0x0000007f, decode(QByteArray("\x7f", 1)));
    EXPECT_EQ(0x00000080, decode(QByteArray("\x81\x00", 2)));
    EXPECT_EQ(0x00003fff, decode(QByteArray("\xff\x7f", 2)));
    EXPECT_EQ(0x00004000, decode(QByteArray("\x81\x80\x00", 3)));
    EXPECT_EQ(0x00004001, decode(QByteArray("\x81\x80\x01", 3)));
    EXPECT_EQ(0x00004083, decode(QByteArray("\x81\x81\x03", 3)));
    EXPECT_EQ(0x001fffff, decode(QByteArray("\xff\xff\x7f", 3)));
    EXPECT_EQ(0x00200000, decode(QByteArray("\x81\x80\x80\x00", 4)));
    EXPECT_EQ(0x00200001, decode(QByteArray("\x81\x80\x80\x01", 4)));
    EXPECT_EQ(0x00200083, decode(QByteArray("\x81\x80\x81\x03", 4)));
    EXPECT_EQ(0x00204187, decode(QByteArray("\x81\x81\x83\x07", 4)));
    EXPECT_EQ(0x0fffffff, decode(QByteArray("\xff\xff\xff\x7f", 4)));
    EXPECT_EQ(0x10000000, decode(QByteArray("\x81\x80\x80\x80\x00", 5)));
    EXPECT_EQ(0x10000001, decode(QByteArray("\x81\x80\x80\x80\x01", 5)));
    EXPECT_EQ(0x10000083, decode(QByteArray("\x81\x80\x80\x81\x03", 5)));
    EXPECT_EQ(0x10004187, decode(QByteArray("\x81\x80\x81\x83\x07", 5)));
    EXPECT_EQ(0x1020c38f, decode(QByteArray("\x81\x81\x83\x87\x0f", 5)));
    EXPECT_EQ(0xffffffff, decode(QByteArray("\x8f\xff\xff\xff\x7f", 5)));
}
