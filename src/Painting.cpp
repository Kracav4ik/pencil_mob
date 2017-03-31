#include "Painting.h"
#include <QPainter>


Stroke::Stroke(const QColor& color, const QPolygon& polygon)
        : color(color), polygon(polygon) {}

Painting::Painting(QObject* parent)
        : QObject(parent), penColor(qrand()%256, qrand()%256, qrand()%256) {}

const QColor& Painting::getPenColor() const {
    return penColor;
}

void Painting::setPenColor(const QColor& color) {
    penColor = color;
}

void Painting::addStroke(const Stroke& stroke) {
    strokes << stroke;
    emit changed();
}

QPicture Painting::getPicture() const {
    QPicture picture;
    QPainter p(&picture);

    for (const Stroke& stroke : strokes) {
        QPen pen(QBrush(stroke.color), 5);
        p.setPen(pen);
        p.drawPolyline(stroke.polygon);
    }
    return picture;
}

void Painting::beginStroke() {
    strokes << Stroke(penColor);
    emit changed();
}

void Painting::continueStroke(const QPoint& point) {
    strokes.last().polygon << point;
    emit changed();
}

void Painting::endStroke() {
    emit strokeFinished(strokes.last());
}

int Painting::strokesSize() const {
    return strokes.size();
}
