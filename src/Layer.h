#pragma once

#include <QImage>
#include "Stroke.h"

class Layer {
private:
    QList<Stroke> strokes;

public:
    QImage drawImg(const QSize& size) const;

    int strokesCount() const;

    void addStroke(const Stroke& stroke);
};
