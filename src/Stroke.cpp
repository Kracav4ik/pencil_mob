#include "Stroke.h"

#include <QPainter>
#include <QJsonObject>
#include <QJsonArray>

Stroke::Stroke(const QColor& color, bool isEraser, const QPolygon& polygon, int brushSize)
        : color(color), isEraser(isEraser), polygon(polygon), brushSize(brushSize) {}

void Stroke::paint(QPainter& painter) const {
    if (isEraser){
        painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);
    } else {
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    }

    QPen pen(QBrush(color), brushSize, Qt::SolidLine, Qt::RoundCap);
    painter.setPen(pen);
    painter.drawPoints(polygon);
    painter.drawPolyline(polygon);
}

void Stroke::read(const QJsonObject& json) {
    QJsonArray array = json["color"].toArray();
    int r = array[0].toInt(-1);
    int g = array[1].toInt(-1);
    int b = array[2].toInt(-1);
    int a = array[3].toInt(-1);
    color = QColor(r, g, b, a);
    brushSize = json["brush_size"].toInt(-1);
    isEraser = json["is_eraser"].toBool(false);
    QPolygon p;
    for (const QJsonValueRef& arr : json["points"].toArray()) {
        QJsonArray point = arr.toArray();
        int x = point[0].toInt(-1);
        int y = point[1].toInt(-1);
        p << QPoint(x, y);
    }
    polygon = p;
}

void Stroke::write(QJsonObject& json) const {
    json["color"] = QJsonArray({color.red(), color.green(), color.blue(), color.alpha()});
    json["brush_size"] = brushSize;
    json["is_eraser"] = isEraser;
    
    QJsonArray newPoints;

    for (const QPoint& point : polygon) {
        newPoints << QJsonArray({point.x(), point.y()});
    }

    json["points"] = newPoints;
}

QDebug operator<<(QDebug debug, const Stroke& stroke) {
    return debug << "color is" << stroke.color << "\n points is" << stroke.polygon.toList();
}
