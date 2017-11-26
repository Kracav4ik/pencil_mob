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

Layer::Layer(const QString& name): name(name) {}

void Layer::copyFrom(const Layer& other) {
    strokes = other.strokes;
}

const QVector<Stroke>& Layer::getSrokes() const {
    return strokes;
}
