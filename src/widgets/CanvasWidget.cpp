#include <QtCore/QElapsedTimer>
#include "widgets/CanvasWidget.h"

void CanvasWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() & Qt::LeftButton) {
        emit beginDrag();
    }
}

void CanvasWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() & Qt::LeftButton) {
        emit endDrag();
    }
}

void CanvasWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        emit drag(event->pos());
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

    QImage img(rect().size(), QImage::Format_ARGB32_Premultiplied);
    img.fill(Qt::transparent);
    {
        QPainter imagePainter(&img);
        if (painting) {
            imagePainter.drawPicture(0, 0, painting->getPicture());
        }
        if (currentTool) {
            currentTool->paint(imagePainter);
        }
    }

    p.drawImage(rect(), img);

    emit debugInfo(painting ? painting->strokesCount() : 0, (int) timer.elapsed());
}

CanvasWidget::CanvasWidget(QWidget *parent) : QWidget(parent) {}

void CanvasWidget::setPainting(Painting* p) {
    painting = p;
}

void CanvasWidget::setCurrentTool(Tool* tool) {
    currentTool = tool;
}
