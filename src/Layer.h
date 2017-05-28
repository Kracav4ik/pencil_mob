#pragma once

#include <QImage>
#include "Stroke.h"

class Layer {
private:
    QList<Stroke> strokes;
    QString name;

public:
    const QString& getName() const;
    void setName(const QString& name);

    QImage drawImg(const QSize& size) const;

    int strokesCount() const;

    void addStroke(const Stroke& stroke);
    explicit Layer(const QString& name);

    void copyFrom(const Layer& other);
};
