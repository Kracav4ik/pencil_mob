#include "gtest/gtest.h"
#include "messages.h"

#define BYTE_ARRAY(STRING) QByteArray(STRING, sizeof(STRING) - 1)

TEST(messages, encoding) {
    EXPECT_EQ(BYTE_ARRAY("\x01\x01"), StringMessage(QString("")).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x06\x01" "abcde"), StringMessage(QString("abcde")).encodeMessage());

    EXPECT_EQ(BYTE_ARRAY("\x01\x02"), SetClientNameMessage(QString("")).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x06\x02" "abcde"), SetClientNameMessage(QString("abcde")).encodeMessage());

    EXPECT_EQ(BYTE_ARRAY("\x05\x03\x01\x02\x03\x00"), PathMessage(1, 2, 3, {}).encodeMessage());
    EXPECT_EQ(
            BYTE_ARRAY("\x0f\x03\xf1\xf2\xf3\x03\x0a\x14\x64\x81\x48\xce\x10\x81\x9c\x20"),
            PathMessage(241, 242, 243, {{10, 20}, {100, 200}, {10000, 20000}}).encodeMessage()
    );
}

TEST(messages, decoding) {
    EXPECT_EQ(QString(""), StringMessage(BYTE_ARRAY("")).str);
    EXPECT_EQ(QString("abcde"), StringMessage(BYTE_ARRAY("abcde")).str);

    EXPECT_EQ(QString(""), SetClientNameMessage(BYTE_ARRAY("")).name);
    EXPECT_EQ(QString("abcde"), SetClientNameMessage(BYTE_ARRAY("abcde")).name);

    PathMessage m1(BYTE_ARRAY("\x01\x02\x03\x00"));
    EXPECT_EQ(1, m1.r);
    EXPECT_EQ(2, m1.g);
    EXPECT_EQ(3, m1.b);
    QVector<QPoint> p1;
    EXPECT_EQ(p1, m1.points);

    PathMessage m2(BYTE_ARRAY("\xf1\xf2\xf3\x03\x0a\x14\x64\x81\x48\xce\x10\x81\x9c\x20"));
    EXPECT_EQ(241, m2.r);
    EXPECT_EQ(242, m2.g);
    EXPECT_EQ(243, m2.b);
    QVector<QPoint> p2{{10, 20}, {100, 200}, {10000, 20000}};
    EXPECT_EQ(p2, m2.points);
}
