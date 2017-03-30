#include <QtCore/QElapsedTimer>
#include "widgets/CanvasWidget.h"

void CanvasWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() & Qt::LeftButton) {
        drawingStroke = true;
        strokes.append(Stroke(penColor));
    }
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() & Qt::LeftButton) {
        drawingStroke = false;
        emit strokeFinished(strokes.last());
    }
}

void CanvasWidget::mouseMoveEvent(QMouseEvent *event) {
    if (drawingStroke) {
        strokes.last().polygon << event->pos();
        update();
    }
}

void CanvasWidget::paintEvent(QPaintEvent *event) {
    QElapsedTimer timer;
    timer.start();

    QImage pattern(16, 16, QImage::Format_RGB32);
    QPainter localP(&pattern);
    localP.fillRect(rect(), QColor(255, 255, 255));
    QColor color(192, 192, 192);

    localP.fillRect(QRect(0, 0, 8, 8), color);
    localP.fillRect(QRect(8, 8, 8, 8), color);

    QBrush b(pattern);
    QPainter p(this);
    p.fillRect(rect(), b);

    for (const Stroke& stroke : strokes) {
        QPen pen(QBrush(stroke.color), 5);
        p.setPen(pen);
        p.drawPolyline(stroke.polygon);
    }

    emit debugInfo(strokes.size(), (int) timer.elapsed());
}

CanvasWidget::CanvasWidget(QWidget *parent) : QWidget(parent), penColor(qrand()%256, qrand()%256, qrand()%256) {}

void CanvasWidget::addStroke(const Stroke& stroke) {
    strokes << stroke;
    update();
}

const QColor& CanvasWidget::getPenColor() const {
    return penColor;
}

void CanvasWidget::setPenColor(const QColor& color) {
    penColor = color;
}

Stroke::Stroke(const QColor& color, const QPolygon& polygon)
        : color(color), polygon(polygon) {}
