#include "CustomPaintWidget.h"

#include <QPainter>
#include <QMouseEvent>

CustomPaintWidget::CustomPaintWidget(QWidget* parent): QWidget(parent) {
}

void CustomPaintWidget::paintEvent(QPaintEvent* event) {
    if (!paintFunction){
        return;
    }
    QPainter p(this);
    paintFunction(*this, p);
}

void CustomPaintWidget::setPaintFunction(const PaintFunction& p) {
    paintFunction = p;
}

void CustomPaintWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() & Qt::LeftButton) {
        isMousePressed = true;
        QPoint point = event->pos();
        emit mouseDrag(point.x(), point.y());
    }
}

void CustomPaintWidget::mouseMoveEvent(QMouseEvent* event) {
    if (isMousePressed) {
        QPoint point = event->pos();
        emit mouseDrag(point.x(), point.y());
    }

}

void CustomPaintWidget::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() & Qt::LeftButton) {
        isMousePressed = false;
    }
}
