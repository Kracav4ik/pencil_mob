#include "gtest/gtest.h"
#include "transport.h"

#define BYTE_ARRAY(STRING) QByteArray(STRING, sizeof(STRING) - 1)

#define EXPECT_DECODER(NUMBER, COUNT, ARRAY) {              \
    Decoder d(ARRAY);                                       \
    EXPECT_EQ(NUMBER, d.number);                            \
    EXPECT_EQ(COUNT, d.count);                              \
}

TEST(transport, test_encode) {
    EXPECT_EQ(encode(0x00000000), BYTE_ARRAY("\x00"));
    EXPECT_EQ(encode(0x00000001), BYTE_ARRAY("\x01"));
    EXPECT_EQ(encode(0x0000007f), BYTE_ARRAY("\x7f"));
    EXPECT_EQ(encode(0x00000080), BYTE_ARRAY("\x81\x00"));
    EXPECT_EQ(encode(0x00003fff), BYTE_ARRAY("\xff\x7f"));
    EXPECT_EQ(encode(0x00004000), BYTE_ARRAY("\x81\x80\x00"));
    EXPECT_EQ(encode(0x00004001), BYTE_ARRAY("\x81\x80\x01"));
    EXPECT_EQ(encode(0x00004083), BYTE_ARRAY("\x81\x81\x03"));
    EXPECT_EQ(encode(0x001fffff), BYTE_ARRAY("\xff\xff\x7f"));
    EXPECT_EQ(encode(0x00200000), BYTE_ARRAY("\x81\x80\x80\x00"));
    EXPECT_EQ(encode(0x00200001), BYTE_ARRAY("\x81\x80\x80\x01"));
    EXPECT_EQ(encode(0x00200083), BYTE_ARRAY("\x81\x80\x81\x03"));
    EXPECT_EQ(encode(0x00204187), BYTE_ARRAY("\x81\x81\x83\x07"));
    EXPECT_EQ(encode(0x0fffffff), BYTE_ARRAY("\xff\xff\xff\x7f"));
    EXPECT_EQ(encode(0x10000000), BYTE_ARRAY("\x81\x80\x80\x80\x00"));
    EXPECT_EQ(encode(0x10000001), BYTE_ARRAY("\x81\x80\x80\x80\x01"));
    EXPECT_EQ(encode(0x10000083), BYTE_ARRAY("\x81\x80\x80\x81\x03"));
    EXPECT_EQ(encode(0x10004187), BYTE_ARRAY("\x81\x80\x81\x83\x07"));
    EXPECT_EQ(encode(0x1020c38f), BYTE_ARRAY("\x81\x81\x83\x87\x0f"));
    EXPECT_EQ(encode(0xffffffff), BYTE_ARRAY("\x8f\xff\xff\xff\x7f"));
}

TEST(transport, test_decode) {
    EXPECT_DECODER(0x00000000, 1, BYTE_ARRAY("\x00"));
    EXPECT_DECODER(0x00000000, 1, BYTE_ARRAY("\x00\x8f\x0f"));
    EXPECT_DECODER(0x00000001, 1, BYTE_ARRAY("\x01"));
    EXPECT_DECODER(0x0000007f, 1, BYTE_ARRAY("\x7f"));
    EXPECT_DECODER(0x00000080, 2, BYTE_ARRAY("\x81\x00"));
    EXPECT_DECODER(0x00003fff, 2, BYTE_ARRAY("\xff\x7f"));
    EXPECT_DECODER(0x00004000, 3, BYTE_ARRAY("\x81\x80\x00"));
    EXPECT_DECODER(0x00004001, 3, BYTE_ARRAY("\x81\x80\x01"));
    EXPECT_DECODER(0x00004083, 3, BYTE_ARRAY("\x81\x81\x03"));
    EXPECT_DECODER(0x001fffff, 3, BYTE_ARRAY("\xff\xff\x7f"));
    EXPECT_DECODER(0x00200000, 4, BYTE_ARRAY("\x81\x80\x80\x00"));
    EXPECT_DECODER(0x00200001, 4, BYTE_ARRAY("\x81\x80\x80\x01"));
    EXPECT_DECODER(0x00200083, 4, BYTE_ARRAY("\x81\x80\x81\x03"));
    EXPECT_DECODER(0x00204187, 4, BYTE_ARRAY("\x81\x81\x83\x07"));
    EXPECT_DECODER(0x0fffffff, 4, BYTE_ARRAY("\xff\xff\xff\x7f"));
    EXPECT_DECODER(0x10000000, 5, BYTE_ARRAY("\x81\x80\x80\x80\x00"));
    EXPECT_DECODER(0x10000001, 5, BYTE_ARRAY("\x81\x80\x80\x80\x01"));
    EXPECT_DECODER(0x10000083, 5, BYTE_ARRAY("\x81\x80\x80\x81\x03"));
    EXPECT_DECODER(0x10004187, 5, BYTE_ARRAY("\x81\x80\x81\x83\x07"));
    EXPECT_DECODER(0x1020c38f, 5, BYTE_ARRAY("\x81\x81\x83\x87\x0f"));
    EXPECT_DECODER(0xffffffff, 5, BYTE_ARRAY("\x8f\xff\xff\xff\x7f"));
}
