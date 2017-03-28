#include <QPainter>
#include "CustomPaintWidget.h"

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
