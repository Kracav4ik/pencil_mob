#include "Layer.h"
#include <QPainter>

QImage Layer::drawImg(const QSize& size, const QPointF& translation) const {
    QImage img(size, QImage::Format_ARGB32_Premultiplied);
    img.fill(Qt::transparent);
    {
        QPainter p(&img);
        p.translate(translation);
        for (const Stroke& stroke : strokes) {
            stroke.paint(p);
        }
    }
    return img;
}

void Layer::addStroke(const Stroke& stroke) {
    strokes << stroke;
}

int Layer::strokesCount() const {
    return strokes.size();
}

void Layer::setName(const QString& nam) {
    name = nam;
}

const QString& Layer::getName() const {
    return name;
}

uint32_t Layer::getUser() const {
    return user;
}

Layer::Layer(uint32_t user, const QString& name): user(user), name(name) {}

void Layer::copyFrom(const Layer& other) {
    strokes = other.strokes;
}
