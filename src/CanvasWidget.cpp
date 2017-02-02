#include <QtCore/QElapsedTimer>
#include "CanvasWidget.h"

void CanvasWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() & Qt::LeftButton) {
        flag = true;
        trajectory.append(QPolygon());
    }
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() & Qt::LeftButton) {
        flag = false;
    }
}

void CanvasWidget::mouseMoveEvent(QMouseEvent *event) {
    if (flag) {
        trajectory.last() << event->pos();
        update();
    }
}

void CanvasWidget::paintEvent(QPaintEvent *event) {
    QElapsedTimer timer;
    timer.start();

    QPainter p(this);
    p.fillRect(rect(), QColor(127, 0, 127));
    QPen pen(QBrush(QColor(128, 255, 196)), 5);
    p.setPen(pen);
    for (const QPolygon &polygon : trajectory) {
        p.drawPolyline(polygon);
    }

    emit debugInfo(trajectory.size(), (int) timer.elapsed());
}

CanvasWidget::CanvasWidget(QWidget *parent) : QWidget(parent) {}
