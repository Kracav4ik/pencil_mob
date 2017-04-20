#pragma once

#include <QString>
#include <QByteArray>
#include <QPoint>
#include <QVector>

#include "enums.h"
#include "transport.h"

struct StringMessage : MessageBase{
    QString str;

    StringMessage(const QString& str)
            : MessageBase(STRING_MESSAGE), str(str) {}

    QByteArray encodeMessage() const override {
        return createM(type, str.toUtf8());
    }

    StringMessage(const QByteArray& data)
            : MessageBase(STRING_MESSAGE) {
        str = QString(data);
    }
};

struct SetClientNameMessage : MessageBase{
    QString name;

    SetClientNameMessage(const QString& name)
            : MessageBase(SET_CLIENT_NAME_MESSAGE), name(name) {}

    QByteArray encodeMessage() const override {
        return createM(type, name.toUtf8());
    }

    SetClientNameMessage(const QByteArray& data)
            : MessageBase(SET_CLIENT_NAME_MESSAGE) {
        name = QString(data);
    }
};

struct PathMessage : MessageBase{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint32_t layerId;
    bool isEraser;
    QVector<QPoint> points;

    PathMessage(uint8_t r, uint8_t g, uint8_t b, uint32_t layerId, bool isEraser, const QVector<QPoint>& points)
            : MessageBase(PATH_MESSAGE), r(r), g(g), b(b), layerId(layerId), isEraser(isEraser), points(points) {}

    QByteArray encodeMessage() const override {
        QByteArray array;

        array.append(r);

        array.append(g);

        array.append(b);

        array.append(encode((uint32_t)layerId));

        array.append((char)(isEraser ? 1 : 0));

        array.append(encode((uint32_t)points.size()));
        for (const QPoint& pointsItem : points) {
            array.append(encode((uint32_t)pointsItem.x()));
            array.append(encode((uint32_t)pointsItem.y()));
        }

        return createM(type, array);
    }

    PathMessage(const QByteArray& data)
            : MessageBase(PATH_MESSAGE) {
        QByteArray m = data;

        r = (uint8_t)m[0];
        m = m.mid(1);

        g = (uint8_t)m[0];
        m = m.mid(1);

        b = (uint8_t)m[0];
        m = m.mid(1);

        layerId = decodeAndShift(m);

        isEraser = m[0] != '\0';
        m = m.mid(1);

        uint32_t pointsCount;
        pointsCount = decodeAndShift(m);
        for (int _ = 0; _ < pointsCount; ++_) {
        uint32_t x = decodeAndShift(m);
        uint32_t y = decodeAndShift(m);

            points << QPoint(x, y);
        }
    }
};

struct AddNewLayerMessage : MessageBase{
    QString layerName;

    AddNewLayerMessage(const QString& layerName)
            : MessageBase(ADD_NEW_LAYER_MESSAGE), layerName(layerName) {}

    QByteArray encodeMessage() const override {
        return createM(type, layerName.toUtf8());
    }

    AddNewLayerMessage(const QByteArray& data)
            : MessageBase(ADD_NEW_LAYER_MESSAGE) {
        layerName = QString(data);
    }
};

struct RenameLayerMessage : MessageBase{
    uint32_t idx;
    QString layerName;

    RenameLayerMessage(uint32_t idx, const QString& layerName)
            : MessageBase(RENAME_LAYER_MESSAGE), idx(idx), layerName(layerName) {}

    QByteArray encodeMessage() const override {
        QByteArray array;

        array.append(encode((uint32_t)idx));

        array.append(layerName.toUtf8());

        return createM(type, array);
    }

    RenameLayerMessage(const QByteArray& data)
            : MessageBase(RENAME_LAYER_MESSAGE) {
        QByteArray m = data;

        idx = decodeAndShift(m);

        layerName = QString(m);
    }
};
