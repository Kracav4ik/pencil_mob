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
    QPointF delta = event->pos() - curMousePos;
    curMousePos = event->localPos();

    if (event->buttons() & Qt::LeftButton) {
        emit leftDrag(camera.screenToWorld(event->pos()));
    } else if (event->buttons() & Qt::MouseButton::RightButton) {
        emit rightDrag(delta);
    }
}

void CanvasWidget::paintEvent(QPaintEvent *event) {
    QElapsedTimer timer;
    timer.start();

    QImage pattern(16, 16, QImage::Format_RGB32);
    QPainter localP(&pattern);
    localP.fillRect(pattern.rect(), QColor(255, 255, 255));
    QColor color(192, 192, 192);

    QRect worldRect(camera.screenToWorld(rect().topLeft()), camera.screenToWorld(rect().bottomRight()));
    localP.fillRect(QRect(0, 0, 8, 8), color);
    localP.fillRect(QRect(8, 8, 8, 8), color);

    QBrush b(pattern);
    QPainter p(this);
    camera.apply(p);
    p.fillRect(worldRect, b);

    QImage img(worldRect.size(), QImage::Format_ARGB32_Premultiplied);
    img.fill(Qt::transparent);
    {
        QPainter imagePainter(&img);
        if (painting) {
            painting->drawPicture(imagePainter, img.size(), -worldRect.topLeft());
        }
    }

    p.drawImage(worldRect, img);

    emit debugInfo(painting ? painting->strokesCount() : 0, (int) timer.elapsed());
}

void CanvasWidget::zoomCamera(float s) {
    // adjust scale factor with regard to possible clamping inside camera
    float oldScale = camera.getScale();
    camera.setScale(oldScale * s);
    s = camera.getScale() / oldScale;

    // first, move (0, 0) of the level to specified pos
    QPointF delta = curMousePos - camera.getTranslation();
    // then move (0, 0) of the level back with respect to scale change
    delta -= delta * s;
    moveCamera(delta);
}

void CanvasWidget::moveCamera(const QPointF& delta) {
    camera.setTranslation(camera.getTranslation() + delta);
}

CanvasWidget::CanvasWidget(QWidget *parent) : QWidget(parent) {
    setMouseTracking(true);
}

void CanvasWidget::setPainting(Painting* p) {
    painting = p;
}

void CanvasWidget::wheelEvent(QWheelEvent* event) {
    emit mouseWheel(event->angleDelta().y()/120.f);
}
