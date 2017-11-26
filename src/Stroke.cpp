#include "Stroke.h"

#include <QPainter>

Stroke::Stroke(const QColor& color, bool isEraser, const QPolygon& polygon)
        : color(color), isEraser(isEraser), polygon(polygon) {}

void Stroke::paint(QPainter& painter) const {
    if (isEraser){
        painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    } else {
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    }

    QPen pen(QBrush(color), 5);
    painter.setPen(pen);
    painter.drawPolyline(polygon);
}

QDebug operator<<(QDebug debug, const Stroke& stroke) {
    return debug << "color is" << stroke.color << "\n points is" << stroke.polygon.toList();
}
