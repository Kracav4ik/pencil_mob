#pragma once

#include <QString>
#include <QByteArray>
#include <QPoint>
#include <QVector>
#include <QColor>

#include "enums.h"
#include "transport.h"

struct StringMessage : MessageBase{
    QString str;

    explicit StringMessage(const QString& str)
            : MessageBase(STRING_MESSAGE), str(str) {}

    QByteArray encodeMessageBody() const override {
        return str.toUtf8();
    }

    explicit StringMessage(const QByteArray& data)
            : MessageBase(STRING_MESSAGE) {
        str = QString(data);
    }
};

struct SetClientNameMessage : MessageBase{
    QString name;

    explicit SetClientNameMessage(const QString& name)
            : MessageBase(SET_CLIENT_NAME_MESSAGE), name(name) {}

    QByteArray encodeMessageBody() const override {
        return name.toUtf8();
    }

    explicit SetClientNameMessage(const QByteArray& data)
            : MessageBase(SET_CLIENT_NAME_MESSAGE) {
        name = QString(data);
    }
};

struct PathMessage : MessageBase{
    QColor color;
    uint32_t layerId;
    bool isEraser;
    QVector<QPoint> points;

    PathMessage(const QColor& color, uint32_t layerId, bool isEraser, const QVector<QPoint>& points)
            : MessageBase(PATH_MESSAGE), color(color), layerId(layerId), isEraser(isEraser), points(points) {}

    QByteArray encodeMessageBody() const override {
        QByteArray array;

        array.append(static_cast<uint8_t>(color.red()));
        array.append(static_cast<uint8_t>(color.green()));
        array.append(static_cast<uint8_t>(color.blue()));

        array.append(encode(static_cast<uint32_t>(layerId)));

        array.append(static_cast<char>(isEraser ? 1 : 0));

        array.append(encode(static_cast<uint32_t>(points.size())));
        for (const QPoint& pointsItem : points) {
            array.append(encode(static_cast<uint32_t>(pointsItem.x())));
            array.append(encode(static_cast<uint32_t>(pointsItem.y())));
        }

        return array;
    }

    explicit PathMessage(const QByteArray& data)
            : MessageBase(PATH_MESSAGE) {
        QByteArray m = data;

        color.setRed(static_cast<uint8_t>(m[0]));
        m = m.mid(1);
        color.setGreen(static_cast<uint8_t>(m[0]));
        m = m.mid(1);
        color.setBlue(static_cast<uint8_t>(m[0]));
        m = m.mid(1);

        layerId = decodeAndShift(m);

        isEraser = m[0] != '\0';
        m = m.mid(1);

        uint32_t pointsCount;
        pointsCount = decodeAndShift(m);
        for (int _ = 0; _ < pointsCount; ++_) {
            uint32_t x;
            x = decodeAndShift(m);

            uint32_t y;
            y = decodeAndShift(m);

            points << QPoint(x, y);
        }
    }
};

struct AddNewLayerMessage : MessageBase{
    QString layerName;

    explicit AddNewLayerMessage(const QString& layerName)
            : MessageBase(ADD_NEW_LAYER_MESSAGE), layerName(layerName) {}

    QByteArray encodeMessageBody() const override {
        return layerName.toUtf8();
    }

    explicit AddNewLayerMessage(const QByteArray& data)
            : MessageBase(ADD_NEW_LAYER_MESSAGE) {
        layerName = QString(data);
    }
};

struct RenameLayerMessage : MessageBase{
    uint32_t uid;
    QString layerName;

    RenameLayerMessage(uint32_t uid, const QString& layerName)
            : MessageBase(RENAME_LAYER_MESSAGE), uid(uid), layerName(layerName) {}

    QByteArray encodeMessageBody() const override {
        QByteArray array;

        array.append(encode(static_cast<uint32_t>(uid)));

        array.append(layerName.toUtf8());

        return array;
    }

    explicit RenameLayerMessage(const QByteArray& data)
            : MessageBase(RENAME_LAYER_MESSAGE) {
        QByteArray m = data;

        uid = decodeAndShift(m);

        layerName = QString(m);
    }
};

struct MoveLayerMessage : MessageBase{
    uint32_t uid;
    uint32_t newPos;

    MoveLayerMessage(uint32_t uid, uint32_t newPos)
            : MessageBase(MOVE_LAYER_MESSAGE), uid(uid), newPos(newPos) {}

    QByteArray encodeMessageBody() const override {
        QByteArray array;

        array.append(encode(static_cast<uint32_t>(uid)));

        array.append(encode(static_cast<uint32_t>(newPos)));

        return array;
    }

    explicit MoveLayerMessage(const QByteArray& data)
            : MessageBase(MOVE_LAYER_MESSAGE) {
        QByteArray m = data;

        uid = decodeAndShift(m);

        newPos = decodeAndShift(m);
    }
};

struct RemoveLayerMessage : MessageBase{
    uint32_t uid;

    explicit RemoveLayerMessage(uint32_t uid)
            : MessageBase(REMOVE_LAYER_MESSAGE), uid(uid) {}

    QByteArray encodeMessageBody() const override {
        return encode(static_cast<uint32_t>(uid));
    }

    explicit RemoveLayerMessage(const QByteArray& data)
            : MessageBase(REMOVE_LAYER_MESSAGE) {
        QByteArray m = data;

        uid = decodeAndShift(m);
    }
};

struct CopyLayerMessage : MessageBase{
    uint32_t fromUid;
    uint32_t toUid;

    CopyLayerMessage(uint32_t fromUid, uint32_t toUid)
            : MessageBase(COPY_LAYER_MESSAGE), fromUid(fromUid), toUid(toUid) {}

    QByteArray encodeMessageBody() const override {
        QByteArray array;

        array.append(encode(static_cast<uint32_t>(fromUid)));

        array.append(encode(static_cast<uint32_t>(toUid)));

        return array;
    }

    explicit CopyLayerMessage(const QByteArray& data)
            : MessageBase(COPY_LAYER_MESSAGE) {
        QByteArray m = data;

        fromUid = decodeAndShift(m);

        toUid = decodeAndShift(m);
    }
};
