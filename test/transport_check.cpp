#include "gtest/gtest.h"
#include "transport.h"

#include <QList>

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

enum {
    MESSAGE_0,
    MESSAGE_1,
    MESSAGE_2,
    MESSAGE_3,
    MESSAGE_4,
    MESSAGE_5
};
struct TestMessageHandler : MessageHandler {
    QList<int> codes;
    QList<QByteArray> messages;
    using MessageHandler::MessageHandler;

    void append(int code, const QByteArray& message) {
        codes << code;
        messages << message;
    }
    int size() const {
        return messages.size();
    }
};
TEST(transport, reader) {
    TestMessageHandler h = {
            {MESSAGE_1, [&h](const QByteArray& message){ h.append(MESSAGE_1, message); }},
            {MESSAGE_2, [&h](const QByteArray& message){ h.append(MESSAGE_2, message); }},
            {MESSAGE_3, [&h](const QByteArray& message){ h.append(MESSAGE_3, message); }},
            {MESSAGE_4, [&h](const QByteArray& message){ h.append(MESSAGE_4, message); }},
            {MESSAGE_5, [&h](const QByteArray& message){ h.append(MESSAGE_5, message); }}
    };

    QByteArray d1(130, '1');
    QByteArray m1 = createM(MESSAGE_1, d1);

    QByteArray d2(3, (char)'\xfe');
    QByteArray m2 = createM(MESSAGE_2, d2);

    QByteArray d3(10, '3');
    QByteArray m3 = createM(MESSAGE_3, d3);

    QByteArray d4(17000, (char)'\xfc');
    QByteArray m4 = createM(MESSAGE_4, d4);

    QByteArray d5;
    QByteArray m5 = createM(MESSAGE_5, d5);

    MessageReader r;
    r.processBytes(m1.left(1), h);
    EXPECT_EQ(0, h.size());
    r.processBytes(m1.mid(1, 1), h);
    EXPECT_EQ(0, h.size());
    r.processBytes(m1.mid(2, m1.size() - 3), h);
    EXPECT_EQ(0, h.size());
    r.processBytes(m1.right(1), h);
    EXPECT_EQ(1, h.size());
    EXPECT_EQ(MESSAGE_1, h.codes[0]);
    EXPECT_EQ(d1, h.messages[0]);

    r.processBytes(m2 + m3 + m4.left(1), h);
    EXPECT_EQ(3, h.size());
    EXPECT_EQ(MESSAGE_2, h.codes[1]);
    EXPECT_EQ(d2, h.messages[1]);
    EXPECT_EQ(MESSAGE_3, h.codes[2]);
    EXPECT_EQ(d3, h.messages[2]);
    r.processBytes(m4.mid(1, 1), h);
    EXPECT_EQ(3, h.size());
    r.processBytes(m4.mid(2, 1), h);
    EXPECT_EQ(3, h.size());
    r.processBytes(m4.mid(3, 10), h);
    EXPECT_EQ(3, h.size());
    r.processBytes(m4.mid(13, m4.size() - 14), h);
    EXPECT_EQ(3, h.size());
    r.processBytes(m4.right(1) + m5, h);
    EXPECT_EQ(5, h.size());
    EXPECT_EQ(MESSAGE_4, h.codes[3]);
    EXPECT_EQ(d4, h.messages[3]);
    EXPECT_EQ(MESSAGE_5, h.codes[4]);
    EXPECT_EQ(d5, h.messages[4]);
}
