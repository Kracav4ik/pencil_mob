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

    EXPECT_EQ(BYTE_ARRAY("\x03\x06\x00\x00"), MoveLayerMessage(0, 0).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x04\x06\x81\x00\x00"), MoveLayerMessage(128, 0).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x04\x06\x00\x81\x00"), MoveLayerMessage(0, 128).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x06\x06\x81\x00\x81\x8c\x00"), MoveLayerMessage(128, 17920).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x06\x06\x81\x8c\x00\x81\x00"), MoveLayerMessage(17920, 128).encodeMessage());

    EXPECT_EQ(BYTE_ARRAY("\x02\x07\x00"), RemoveLayerMessage(0).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x03\x07\x81\x00"), RemoveLayerMessage(128).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x04\x07\x81\x8c\x00"), RemoveLayerMessage(17920).encodeMessage());

    EXPECT_EQ(BYTE_ARRAY("\x03\x08\x00\x00"), CopyLayerMessage(0, 0).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x04\x08\x81\x00\x00"), CopyLayerMessage(128, 0).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x04\x08\x00\x81\x00"), CopyLayerMessage(0, 128).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x06\x08\x81\x00\x81\x8c\x00"), CopyLayerMessage(128, 17920).encodeMessage());
    EXPECT_EQ(BYTE_ARRAY("\x06\x08\x81\x8c\x00\x81\x00"), CopyLayerMessage(17920, 128).encodeMessage());
}

TEST(messages, decoding) {
    EXPECT_EQ(QString(""), StringMessage(BYTE_ARRAY("")).str);
    EXPECT_EQ(QString("abcde"), StringMessage(BYTE_ARRAY("abcde")).str);

    EXPECT_EQ(QString(""), SetClientNameMessage(BYTE_ARRAY("")).name);
    EXPECT_EQ(QString("abcde"), SetClientNameMessage(BYTE_ARRAY("abcde")).name);

    {
        PathMessage m(BYTE_ARRAY("\x01\x02\x03\x00\x01\x00"));
        EXPECT_EQ(1, m.r);
        EXPECT_EQ(2, m.g);
        EXPECT_EQ(3, m.b);
        EXPECT_EQ(0, m.layerId);
        EXPECT_TRUE(m.isEraser);
        QVector<QPoint> p;
        EXPECT_EQ(p, m.points);
    }

    {
        PathMessage m(BYTE_ARRAY("\xf1\xf2\xf3\x81\x00\x00\x03\x0a\x14\x64\x81\x48\xce\x10\x81\x9c\x20"));
        EXPECT_EQ(241, m.r);
        EXPECT_EQ(242, m.g);
        EXPECT_EQ(243, m.b);
        EXPECT_EQ(128, m.layerId);
        EXPECT_FALSE(m.isEraser);
        QVector<QPoint> p{{10, 20}, {100, 200}, {10000, 20000}};
        EXPECT_EQ(p, m.points);
    }

    EXPECT_EQ(QString(""), AddNewLayerMessage(BYTE_ARRAY("")).layerName);
    EXPECT_EQ(QString("abcde"), AddNewLayerMessage(BYTE_ARRAY("abcde")).layerName);

    {
        RenameLayerMessage m(BYTE_ARRAY("\x00"));
        EXPECT_EQ(0, m.uid);
        EXPECT_EQ(QString(""), m.layerName);
    }

    {
        RenameLayerMessage m(BYTE_ARRAY("\x81\x8c\x00" "qwe"));
        EXPECT_EQ(17920, m.uid);
        EXPECT_EQ(QString("qwe"), m.layerName);
    }

    {
        MoveLayerMessage m(BYTE_ARRAY("\x00\x00"));
        EXPECT_EQ(0, m.uid);
        EXPECT_EQ(0, m.newPos);
    }

    {
        MoveLayerMessage m(BYTE_ARRAY("\x81\x00\x81\x8c\x00"));
        EXPECT_EQ(128, m.uid);
        EXPECT_EQ(17920, m.newPos);
    }

    EXPECT_EQ(0, RemoveLayerMessage(BYTE_ARRAY("\x00")).uid);
    EXPECT_EQ(128, RemoveLayerMessage(BYTE_ARRAY("\x81\x00")).uid);
    EXPECT_EQ(17920, RemoveLayerMessage(BYTE_ARRAY("\x81\x8c\x00")).uid);

    {
        CopyLayerMessage m(BYTE_ARRAY("\x00\x00"));
        EXPECT_EQ(0, m.fromUid);
        EXPECT_EQ(0, m.toUid);
    }

    {
        CopyLayerMessage m(BYTE_ARRAY("\x81\x00\x81\x8c\x00"));
        EXPECT_EQ(128, m.fromUid);
        EXPECT_EQ(17920, m.toUid);
    }
}
