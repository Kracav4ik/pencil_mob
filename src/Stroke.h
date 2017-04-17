#pragma once

#include <QPolygon>
#include <QColor>


class QPainter;

struct Stroke{
    QColor color;
    bool isEraser;
    QPolygon polygon;

    Stroke(const QColor& color, bool isEraser=false, const QPolygon& polygon=QPolygon());

    void paint(QPainter& painter) const;
};



