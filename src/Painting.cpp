#include "Painting.h"
#include <QPainter>

QColor randColor() {
    return QColor(qrand()%256, qrand()%256, qrand()%256);
}

Painting::Painting(QObject* parent)
        : QObject(parent), penColor(randColor()) {}

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

int Painting::strokesCount() const {
    return strokes.size();
}
