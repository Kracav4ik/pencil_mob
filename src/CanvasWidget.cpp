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
    QColor bg = QColor(192, 192, 192);
    if(penColor.value() > 128){
        bg.setRgb(64, 64, 64);
    }
    p.fillRect(rect(), bg);
    QPen pen(QBrush(penColor), 5);
    p.setPen(pen);
    for (const QPolygon &polygon : trajectory) {
        p.drawPolyline(polygon);
    }

    emit debugInfo(trajectory.size(), (int) timer.elapsed());
}

CanvasWidget::CanvasWidget(QWidget *parent) : QWidget(parent), penColor(qrand()%256, qrand()%256, qrand()%256) {}
