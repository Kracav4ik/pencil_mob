#include "gtest/gtest.h"
#include "messages.h"

#define BYTE_ARRAY(STRING) QByteArray(STRING, sizeof(STRING) - 1)

// TODO: add user messages encoding/decoding tests

TEST(messages, encoding_anonymous) {
    EXPECT_EQ(BYTE_ARRAY("\x01\x01"), StringMessage(QString("")).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x06\x01" "abcde"), StringMessage(QString("abcde")).encodeMessage());

    EXPECT_EQ(BYTE_ARRAY("\x01\x02"), SetClientInfoMessage(QString("")).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x06\x02" "abcde"), SetClientInfoMessage(QString("abcde")).encodeMessage());

    EXPECT_EQ(BYTE_ARRAY("\x08\x03\x01\x02\x03\x81\x00\x00\x00"), PathMessage(QColor(1, 2, 3), 128, false, {}).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x09\x03\x01\x02\x03\x81\x8c\x00\x01\x00"), PathMessage(QColor(1, 2, 3), 17920, true, {}).encodeMessage());
    EXPECT_EQ(
            BYTE_ARRAY("\x11\x03\xf1\xf2\xf3\x00\x01\x03\x0a\x14\x64\x81\x48\xce\x10\x81\x9c\x20"),
            PathMessage(QColor(241, 242, 243), 0, true, {{10, 20}, {100, 200}, {10000, 20000}}).encodeMessage()
    );

    EXPECT_EQ(BYTE_ARRAY("\x02\x04\00"), AddNewLayerMessage(0, QString()).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x09\x04\x7f" "abcdefg"), AddNewLayerMessage(127, QString("abcdefg")).encodeMessage());

    EXPECT_EQ(BYTE_ARRAY("\x02\x05\x00"), RenameLayerMessage(0, QString("")).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x05\x05\x00" "qwe"), RenameLayerMessage(0, QString("qwe")).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x06\x05\x81\x00" "qwe"), RenameLayerMessage(128, QString("qwe")).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x07\x05\x81\x8c\x00" "qwe"), RenameLayerMessage(17920, QString("qwe")).encodeMessage());

    EXPECT_EQ(BYTE_ARRAY("\x03\x06\x00\x00"), MoveLayerMessage(0, 0).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x04\x06\x81\x00\x00"), MoveLayerMessage(128, 0).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x04\x06\x00\x81\x00"), MoveLayerMessage(0, 128).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x06\x06\x81\x00\x81\x8c\x00"), MoveLayerMessage(128, 17920).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x06\x06\x81\x8c\x00\x81\x00"), MoveLayerMessage(17920, 128).encodeMessage());

    EXPECT_EQ(BYTE_ARRAY("\x02\x07\x00"), RemoveLayerMessage(0).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x03\x07\x81\x00"), RemoveLayerMessage(128).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x04\x07\x81\x8c\x00"), RemoveLayerMessage(17920).encodeMessage());

    EXPECT_EQ(BYTE_ARRAY("\x04\x09\x00\x00\x00"), CopyLayerMessage(0, 0, 0).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x05\x09\00\x81\x00\x00"), CopyLayerMessage(0, 128, 0).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x06\x09\x81\x00\x00\x81\x00"), CopyLayerMessage(128, 0, 128).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x09\x09\x81\x8c\x00\x81\x00\x81\x8c\x00"), CopyLayerMessage(17920, 128, 17920).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x07\x09\00\x81\x8c\x00\x81\x00"), CopyLayerMessage(0, 17920, 128).encodeMessage());
}

TEST(messages, decoding_anonymous) {
    EXPECT_EQ(QString(""), StringMessage(BYTE_ARRAY("")).str);
    EXPECT_EQ(QString("abcde"), StringMessage(BYTE_ARRAY("abcde")).str);

    EXPECT_EQ(QString(""), SetClientInfoMessage(BYTE_ARRAY("")).name);
    EXPECT_EQ(QString("abcde"), SetClientInfoMessage(BYTE_ARRAY("abcde")).name);

    {
        PathMessage m(BYTE_ARRAY("\x01\x02\x03\x00\x01\x00"));
        EXPECT_EQ(1, m.color.red());
        EXPECT_EQ(2, m.color.green());
        EXPECT_EQ(3, m.color.blue());
        EXPECT_EQ(0, m.layerId);
        EXPECT_TRUE(m.isEraser);
        QVector<QPoint> p;
        EXPECT_EQ(p, m.points);
    }

    {
        PathMessage m(BYTE_ARRAY("\xf1\xf2\xf3\x81\x00\x00\x03\x0a\x14\x64\x81\x48\xce\x10\x81\x9c\x20"));
        EXPECT_EQ(241, m.color.red());
        EXPECT_EQ(242, m.color.green());
        EXPECT_EQ(243, m.color.blue());
        EXPECT_EQ(128, m.layerId);
        EXPECT_FALSE(m.isEraser);
        QVector<QPoint> p{{10, 20}, {100, 200}, {10000, 20000}};
        EXPECT_EQ(p, m.points);
    }

    EXPECT_EQ(QString(""), AddNewLayerMessage(BYTE_ARRAY("\00""")).layerName);
    EXPECT_EQ(QString("abcde"), AddNewLayerMessage(BYTE_ARRAY("\00""abcde")).layerName);

    {
        RenameLayerMessage m(BYTE_ARRAY("\x00"));
        EXPECT_EQ(0, m.layerId);
        EXPECT_EQ(QString(""), m.layerName);
    }

    {
        RenameLayerMessage m(BYTE_ARRAY("\x81\x8c\x00" "qwe"));
        EXPECT_EQ(17920, m.layerId);
        EXPECT_EQ(QString("qwe"), m.layerName);
    }

    {
        MoveLayerMessage m(BYTE_ARRAY("\x00\x00"));
        EXPECT_EQ(0, m.layerId);
        EXPECT_EQ(0, m.newPos);
    }

    {
        MoveLayerMessage m(BYTE_ARRAY("\x81\x00\x81\x8c\x00"));
        EXPECT_EQ(128, m.layerId);
        EXPECT_EQ(17920, m.newPos);
    }

    EXPECT_EQ(0, RemoveLayerMessage(BYTE_ARRAY("\x00")).layerId);
    EXPECT_EQ(128, RemoveLayerMessage(BYTE_ARRAY("\x81\x00")).layerId);
    EXPECT_EQ(17920, RemoveLayerMessage(BYTE_ARRAY("\x81\x8c\x00")).layerId);

    {
        CopyLayerMessage m(BYTE_ARRAY("\x00\x00\x00"));
        EXPECT_EQ(0, m.fromUserId);
        EXPECT_EQ(0, m.fromLayerId);
        EXPECT_EQ(0, m.toLayerId);
    }

    {
        CopyLayerMessage m(BYTE_ARRAY("\x00\x81\x00\x81\x8c\x00"));
        EXPECT_EQ(0, m.fromUserId);
        EXPECT_EQ(128, m.fromLayerId);
        EXPECT_EQ(17920, m.toLayerId);
    }
}
