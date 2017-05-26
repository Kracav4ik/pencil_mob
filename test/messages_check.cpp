#include "gtest/gtest.h"
#include "messages.h"

#define BYTE_ARRAY(STRING) QByteArray(STRING, sizeof(STRING) - 1)

TEST(messages, encoding) {
    EXPECT_EQ(BYTE_ARRAY("\x01\x01"), StringMessage(QString("")).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x06\x01" "abcde"), StringMessage(QString("abcde")).encodeMessage());

    EXPECT_EQ(BYTE_ARRAY("\x01\x02"), SetClientNameMessage(QString("")).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x06\x02" "abcde"), SetClientNameMessage(QString("abcde")).encodeMessage());

    EXPECT_EQ(BYTE_ARRAY("\x08\x03\x01\x02\x03\x81\x00\x00\x00"), PathMessage(1, 2, 3, 128, false, {}).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x09\x03\x01\x02\x03\x81\x8c\x00\x01\x00"), PathMessage(1, 2, 3, 17920, true, {}).encodeMessage());
    EXPECT_EQ(
            BYTE_ARRAY("\x11\x03\xf1\xf2\xf3\x00\x01\x03\x0a\x14\x64\x81\x48\xce\x10\x81\x9c\x20"),
            PathMessage(241, 242, 243, 0, true, {{10, 20}, {100, 200}, {10000, 20000}}).encodeMessage()
    );

    EXPECT_EQ(BYTE_ARRAY("\x01\x04"), AddNewLayerMessage(QString()).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x08\x04" "abcdefg"), AddNewLayerMessage(QString("abcdefg")).encodeMessage());

    EXPECT_EQ(BYTE_ARRAY("\x02\x05\x00"), RenameLayerMessage(0, QString("")).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x05\x05\x00" "qwe"), RenameLayerMessage(0, QString("qwe")).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x06\x05\x81\x00" "qwe"), RenameLayerMessage(128, QString("qwe")).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x07\x05\x81\x8c\x00" "qwe"), RenameLayerMessage(17920, QString("qwe")).encodeMessage());
}

TEST(messages, decoding) {
    EXPECT_EQ(QString(""), StringMessage(BYTE_ARRAY("")).str);
    EXPECT_EQ(QString("abcde"), StringMessage(BYTE_ARRAY("abcde")).str);

    EXPECT_EQ(QString(""), SetClientNameMessage(BYTE_ARRAY("")).name);
    EXPECT_EQ(QString("abcde"), SetClientNameMessage(BYTE_ARRAY("abcde")).name);

    PathMessage m1(BYTE_ARRAY("\x01\x02\x03\x00\x01\x00"));
    EXPECT_EQ(1, m1.r);
    EXPECT_EQ(2, m1.g);
    EXPECT_EQ(3, m1.b);
    EXPECT_EQ(0, m1.layerId);
    EXPECT_TRUE(m1.isEraser);
    QVector<QPoint> p1;
    EXPECT_EQ(p1, m1.points);

    PathMessage m2(BYTE_ARRAY("\xf1\xf2\xf3\x81\x00\x00\x03\x0a\x14\x64\x81\x48\xce\x10\x81\x9c\x20"));
    EXPECT_EQ(241, m2.r);
    EXPECT_EQ(242, m2.g);
    EXPECT_EQ(243, m2.b);
    EXPECT_EQ(128, m2.layerId);
    EXPECT_FALSE(m2.isEraser);
    QVector<QPoint> p2{{10, 20}, {100, 200}, {10000, 20000}};
    EXPECT_EQ(p2, m2.points);

    EXPECT_EQ(QString(""), AddNewLayerMessage(BYTE_ARRAY("")).layerName);
    EXPECT_EQ(QString("abcde"), AddNewLayerMessage(BYTE_ARRAY("abcde")).layerName);

    RenameLayerMessage m3 = RenameLayerMessage(BYTE_ARRAY("\x00"));
    EXPECT_EQ(0, m3.uid);
    EXPECT_EQ(QString(""), m3.layerName);

    RenameLayerMessage m4 = RenameLayerMessage(BYTE_ARRAY("\x81\x8c\x00" "qwe"));
    EXPECT_EQ(17920, m4.uid);
    EXPECT_EQ(QString("qwe"), m4.layerName);
}
