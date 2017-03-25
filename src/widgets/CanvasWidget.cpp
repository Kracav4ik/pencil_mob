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

    QPainter p(this);
    QColor bg = QColor(192, 192, 192);
    if(penColor.value() > 128){
        bg.setRgb(64, 64, 64);
    }
    p.fillRect(rect(), bg);
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

Stroke::Stroke(const QColor& color, const QPolygon& polygon)
        : color(color), polygon(polygon) {}
