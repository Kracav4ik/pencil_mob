#pragma once

#include <QByteArray>
#include <QString>

QByteArray encode(uint32_t value);
struct Decoder{
    uint32_t number;
    int count;

    Decoder(QByteArray bytes);
};

QByteArray createM(uint32_t type, QString string);

QString takeM(QByteArray message);