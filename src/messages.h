#pragma once

#include <QString>
#include <QByteArray>
#include <QPoint>
#include <QVector>
#include <QColor>
#include <QPolygon>

#include "enums.h"
#include "transport.h"
#include "Stroke.h"

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

struct SetClientInfoMessage : MessageBase{
    QString name;

    explicit SetClientInfoMessage(const QString& name)
            : MessageBase(SET_CLIENT_INFO_MESSAGE), name(name) {}

    QByteArray encodeMessageBody() const override {
        return name.toUtf8();
    }

    explicit SetClientInfoMessage(const QByteArray& data)
            : MessageBase(SET_CLIENT_INFO_MESSAGE) {
        name = QString(data);
    }
};

struct PathMessage : MessageBase{
    QColor color;
    uint32_t layerId;
    bool isEraser;
    QVector<QPoint> points;
    uint32_t brushSize;

    PathMessage(const QColor& color, uint32_t layerId, bool isEraser, const QVector<QPoint>& points, uint32_t brushSize)
            : MessageBase(PATH_MESSAGE), color(color), layerId(layerId), isEraser(isEraser), points(points), brushSize(brushSize) {}

    QByteArray encodeMessageBody() const override {
        QByteArray array;

        array.append(static_cast<uint8_t>(color.red()));
        array.append(static_cast<uint8_t>(color.green()));
        array.append(static_cast<uint8_t>(color.blue()));
        array.append(static_cast<uint8_t>(color.alpha()));

        array.append(encode(static_cast<uint32_t>(layerId)));

        array.append(static_cast<char>(isEraser ? 1 : 0));

        array.append(encode(static_cast<uint32_t>(points.size())));
        for (const QPoint& pointsItem : points) {
            array.append(encode(static_cast<uint32_t>(pointsItem.x())));
            array.append(encode(static_cast<uint32_t>(pointsItem.y())));
        }

        array.append(encode(static_cast<uint32_t>(brushSize)));

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
        color.setAlpha(static_cast<uint8_t>(m[0]));
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

        brushSize = decodeAndShift(m);
    }
};

struct AddNewLayerMessage : MessageBase{
    uint32_t layerId;
    QString layerName;

    AddNewLayerMessage(uint32_t layerId, const QString& layerName)
            : MessageBase(ADD_NEW_LAYER_MESSAGE), layerId(layerId), layerName(layerName) {}

    QByteArray encodeMessageBody() const override {
        QByteArray array;

        array.append(encode(static_cast<uint32_t>(layerId)));

        array.append(layerName.toUtf8());

        return array;
    }

    explicit AddNewLayerMessage(const QByteArray& data)
            : MessageBase(ADD_NEW_LAYER_MESSAGE) {
        QByteArray m = data;

        layerId = decodeAndShift(m);

        layerName = QString(m);
    }
};

struct RenameLayerMessage : MessageBase{
    uint32_t layerId;
    QString layerName;

    RenameLayerMessage(uint32_t layerId, const QString& layerName)
            : MessageBase(RENAME_LAYER_MESSAGE), layerId(layerId), layerName(layerName) {}

    QByteArray encodeMessageBody() const override {
        QByteArray array;

        array.append(encode(static_cast<uint32_t>(layerId)));

        array.append(layerName.toUtf8());

        return array;
    }

    explicit RenameLayerMessage(const QByteArray& data)
            : MessageBase(RENAME_LAYER_MESSAGE) {
        QByteArray m = data;

        layerId = decodeAndShift(m);

        layerName = QString(m);
    }
};

struct MoveLayerMessage : MessageBase{
    uint32_t layerId;
    uint32_t newPos;

    MoveLayerMessage(uint32_t layerId, uint32_t newPos)
            : MessageBase(MOVE_LAYER_MESSAGE), layerId(layerId), newPos(newPos) {}

    QByteArray encodeMessageBody() const override {
        QByteArray array;

        array.append(encode(static_cast<uint32_t>(layerId)));

        array.append(encode(static_cast<uint32_t>(newPos)));

        return array;
    }

    explicit MoveLayerMessage(const QByteArray& data)
            : MessageBase(MOVE_LAYER_MESSAGE) {
        QByteArray m = data;

        layerId = decodeAndShift(m);

        newPos = decodeAndShift(m);
    }
};

struct RemoveLayerMessage : MessageBase{
    uint32_t layerId;

    explicit RemoveLayerMessage(uint32_t layerId)
            : MessageBase(REMOVE_LAYER_MESSAGE), layerId(layerId) {}

    QByteArray encodeMessageBody() const override {
        return encode(static_cast<uint32_t>(layerId));
    }

    explicit RemoveLayerMessage(const QByteArray& data)
            : MessageBase(REMOVE_LAYER_MESSAGE) {
        QByteArray m = data;

        layerId = decodeAndShift(m);
    }
};

struct RemoveLastStrokeMessage : MessageBase{
    uint32_t layerId;

    explicit RemoveLastStrokeMessage(uint32_t layerId)
            : MessageBase(REMOVE_LAST_STROKE_MESSAGE), layerId(layerId) {}

    QByteArray encodeMessageBody() const override {
        return encode(static_cast<uint32_t>(layerId));
    }

    explicit RemoveLastStrokeMessage(const QByteArray& data)
            : MessageBase(REMOVE_LAST_STROKE_MESSAGE) {
        QByteArray m = data;

        layerId = decodeAndShift(m);
    }
};

struct CopyLayerMessage : MessageBase{
    uint32_t fromUserId;
    uint32_t fromLayerId;
    uint32_t toLayerId;

    CopyLayerMessage(uint32_t fromUserId, uint32_t fromLayerId, uint32_t toLayerId)
            : MessageBase(COPY_LAYER_MESSAGE), fromUserId(fromUserId), fromLayerId(fromLayerId), toLayerId(toLayerId) {}

    QByteArray encodeMessageBody() const override {
        QByteArray array;

        array.append(encode(static_cast<uint32_t>(fromUserId)));

        array.append(encode(static_cast<uint32_t>(fromLayerId)));

        array.append(encode(static_cast<uint32_t>(toLayerId)));

        return array;
    }

    explicit CopyLayerMessage(const QByteArray& data)
            : MessageBase(COPY_LAYER_MESSAGE) {
        QByteArray m = data;

        fromUserId = decodeAndShift(m);

        fromLayerId = decodeAndShift(m);

        toLayerId = decodeAndShift(m);
    }
};

struct LayerContentsMessage : MessageBase{
    uint32_t layerId;
    QVector<Stroke> strokes;
    QString layerName;

    LayerContentsMessage(uint32_t layerId, const QVector<Stroke>& strokes, const QString& layerName)
            : MessageBase(LAYER_CONTENTS_MESSAGE), layerId(layerId), strokes(strokes), layerName(layerName) {}

    QByteArray encodeMessageBody() const override {
        QByteArray array;

        array.append(encode(static_cast<uint32_t>(layerId)));

        array.append(encode(static_cast<uint32_t>(strokes.size())));
        for (const Stroke& strokesItem : strokes) {
            array.append(static_cast<uint8_t>(strokesItem.color.red()));
            array.append(static_cast<uint8_t>(strokesItem.color.green()));
            array.append(static_cast<uint8_t>(strokesItem.color.blue()));
            array.append(static_cast<uint8_t>(strokesItem.color.alpha()));
            array.append(static_cast<char>(strokesItem.isEraser ? 1 : 0));
            array.append(encode(static_cast<uint32_t>(strokesItem.polygon.size())));
            for (const QPoint& strokesItem_polygonItem : strokesItem.polygon) {
                array.append(encode(static_cast<uint32_t>(strokesItem_polygonItem.x())));
                array.append(encode(static_cast<uint32_t>(strokesItem_polygonItem.y())));
            }
            array.append(encode(static_cast<uint32_t>(strokesItem.brushSize)));
        }

        array.append(layerName.toUtf8());

        return array;
    }

    explicit LayerContentsMessage(const QByteArray& data)
            : MessageBase(LAYER_CONTENTS_MESSAGE) {
        QByteArray m = data;

        layerId = decodeAndShift(m);

        uint32_t strokesCount;
        strokesCount = decodeAndShift(m);
        for (int _ = 0; _ < strokesCount; ++_) {
            QColor color;
            color.setRed(static_cast<uint8_t>(m[0]));
        m = m.mid(1);
        color.setGreen(static_cast<uint8_t>(m[0]));
        m = m.mid(1);
        color.setBlue(static_cast<uint8_t>(m[0]));
        m = m.mid(1);
        color.setAlpha(static_cast<uint8_t>(m[0]));
        m = m.mid(1);

            bool isEraser;
            isEraser = m[0] != '\0';
        m = m.mid(1);

            QPolygon polygon;
            uint32_t polygonCount;
        polygonCount = decodeAndShift(m);
        for (int _ = 0; _ < polygonCount; ++_) {
            uint32_t x;
            x = decodeAndShift(m);

            uint32_t y;
            y = decodeAndShift(m);

            polygon << QPoint(x, y);
        }

            uint32_t brushSize;
            brushSize = decodeAndShift(m);

            strokes << Stroke(color, isEraser, polygon, brushSize);
        }

        layerName = QString(m);
    }
};
