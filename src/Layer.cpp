#include "Layer.h"
#include <QPainter>

QImage Layer::drawImg(const QSize& size) const {
    QImage img(size, QImage::Format_ARGB32_Premultiplied);
    img.fill(Qt::transparent);
    {
        QPainter p(&img);
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

Layer::Layer(const QString& name): name(name) {}
