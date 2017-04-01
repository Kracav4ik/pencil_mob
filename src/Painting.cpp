#include "Painting.h"
#include <QPainter>


Stroke::Stroke(const QColor& color, const QPolygon& polygon)
        : color(color), polygon(polygon) {}

void Stroke::paint(QPainter& painter) const {
    QPen pen(QBrush(color), 5);
    painter.setPen(pen);
    painter.drawPolyline(polygon);
}

Painting::Painting(QObject* parent)
        : QObject(parent), penColor(qrand()%256, qrand()%256, qrand()%256) {}

const QColor& Painting::getPenColor() const {
    return penColor;
}

void Painting::setPenColor(const QColor& color) {
    penColor = color;
    emit penColorChanged(penColor);
}

void Painting::addStroke(const Stroke& stroke) {
    strokes << stroke;
    emit changed();
}

QPicture Painting::getPicture() const {
    QPicture picture;
    QPainter p(&picture);

    for (const Stroke& stroke : strokes) {
        stroke.paint(p);
    }
    return picture;
}

int Painting::strokesSize() const {
    return strokes.size();
}
