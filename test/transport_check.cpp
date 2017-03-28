#include "gtest/gtest.h"
#include "transport.h"

#define BYTE_ARRAY(STRING) QByteArray(STRING, sizeof(STRING) - 1)

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
    EXPECT_EQ(0x00000000, Decoder(BYTE_ARRAY("\x00")).number);
    EXPECT_EQ(0x00000000, Decoder(BYTE_ARRAY("\x00\x8f\x0f")).number);
    EXPECT_EQ(0x00000001, Decoder(BYTE_ARRAY("\x01")).number);
    EXPECT_EQ(0x0000007f, Decoder(BYTE_ARRAY("\x7f")).number);
    EXPECT_EQ(0x00000080, Decoder(BYTE_ARRAY("\x81\x00")).number);
    EXPECT_EQ(0x00003fff, Decoder(BYTE_ARRAY("\xff\x7f")).number);
    EXPECT_EQ(0x00004000, Decoder(BYTE_ARRAY("\x81\x80\x00")).number);
    EXPECT_EQ(0x00004001, Decoder(BYTE_ARRAY("\x81\x80\x01")).number);
    EXPECT_EQ(0x00004083, Decoder(BYTE_ARRAY("\x81\x81\x03")).number);
    EXPECT_EQ(0x001fffff, Decoder(BYTE_ARRAY("\xff\xff\x7f")).number);
    EXPECT_EQ(0x00200000, Decoder(BYTE_ARRAY("\x81\x80\x80\x00")).number);
    EXPECT_EQ(0x00200001, Decoder(BYTE_ARRAY("\x81\x80\x80\x01")).number);
    EXPECT_EQ(0x00200083, Decoder(BYTE_ARRAY("\x81\x80\x81\x03")).number);
    EXPECT_EQ(0x00204187, Decoder(BYTE_ARRAY("\x81\x81\x83\x07")).number);
    EXPECT_EQ(0x0fffffff, Decoder(BYTE_ARRAY("\xff\xff\xff\x7f")).number);
    EXPECT_EQ(0x10000000, Decoder(BYTE_ARRAY("\x81\x80\x80\x80\x00")).number);
    EXPECT_EQ(0x10000001, Decoder(BYTE_ARRAY("\x81\x80\x80\x80\x01")).number);
    EXPECT_EQ(0x10000083, Decoder(BYTE_ARRAY("\x81\x80\x80\x81\x03")).number);
    EXPECT_EQ(0x10004187, Decoder(BYTE_ARRAY("\x81\x80\x81\x83\x07")).number);
    EXPECT_EQ(0x1020c38f, Decoder(BYTE_ARRAY("\x81\x81\x83\x87\x0f")).number);
    EXPECT_EQ(0xffffffff, Decoder(BYTE_ARRAY("\x8f\xff\xff\xff\x7f")).number);
}
