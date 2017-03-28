#include "gtest/gtest.h"
#include "transport.h"

struct TestByteArray {
    QByteArray array;
    TestByteArray(const char* string, int size) : array(string, size) {}
    int size() const { return array.size(); }
    char operator[](int index) const {
        EXPECT_GE(index, 0);
        EXPECT_LT(index, size());
        return array[index];
    }
};
bool operator==(const QByteArray& qba, const TestByteArray& tba) {
    return qba == tba.array;
}
#define BYTE_ARRAY(STRING) TestByteArray(STRING, sizeof(STRING) - 1)

#define _DO_EXPECT_DECODER(NUMBER, COUNT, EXPECT_DECODED, ARRAY) {  \
    Decoder d(ARRAY);                                               \
    EXPECT_EQ(NUMBER, d.number);                                    \
    EXPECT_EQ(COUNT, d.count);                                      \
    EXPECT_DECODED(d.decoded);                                      \
}
#define EXPECT_DECODER(NUMBER, COUNT, ARRAY) _DO_EXPECT_DECODER(NUMBER, COUNT, EXPECT_TRUE, ARRAY)
#define EXPECT_DECODER_FAIL(ARRAY) _DO_EXPECT_DECODER(0x00000000, 0, EXPECT_FALSE, ARRAY)

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

TEST(transport, test_decode_incomplete) {
    EXPECT_DECODER_FAIL(BYTE_ARRAY("\x81"));
    EXPECT_DECODER_FAIL(BYTE_ARRAY("\x81\x81"));
    EXPECT_DECODER_FAIL(BYTE_ARRAY("\x81\x81\x83"));
    EXPECT_DECODER_FAIL(BYTE_ARRAY("\x8f\xff\xff\xff"));
    // TODO: decide what decoder shall return for overflow
//    EXPECT_DECODER_FAIL(BYTE_ARRAY("\x90\x80\x80\x80\x00"));
//    EXPECT_DECODER_FAIL(BYTE_ARRAY("\x90\x80\x80\x80\x01"));
    EXPECT_DECODER_FAIL(BYTE_ARRAY("\xff\xff\xff\xff\xff"));
    EXPECT_DECODER_FAIL(BYTE_ARRAY("\xff\xff\xff\xff\xff\xff"));
}