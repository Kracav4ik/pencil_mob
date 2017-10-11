#include "Camera.h"

#include <QPainter>

static const float MAX_SCALE = 8;

float Camera::getScale() const {
    return scale;
}

void Camera::setScale(float s) {
    scale = qBound(1/MAX_SCALE, s, 2*MAX_SCALE);
}

const QPointF& Camera::getTranslation() const {
    return translation;
}

void Camera::setTranslation(const QPointF& p) {
    translation = p;
}

void Camera::apply(QPainter& p) const {
    p.translate(translation);
    p.scale(scale, scale);
}

QPoint Camera::screenToWorld(const QPoint& pos) const {
    return ((pos - translation)/scale).toPoint();
}

QPoint Camera::worldToScreen(const QPoint& pos) const {
    return (pos*scale + translation).toPoint();
}
