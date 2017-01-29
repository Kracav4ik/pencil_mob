#pragma once

#include <QByteArray>

QByteArray encode(uint32_t value);


uint32_t decode(QByteArray bytes);