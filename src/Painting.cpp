#include "Painting.h"
#include <QPainter>
#include "tools/Tool.h"
#include "Stroke.h"
#include "Layer.h"

QColor randColor() {
    return QColor(qrand()%256, qrand()%256, qrand()%256);
}

Painting::Painting(QObject* parent)
        : QObject(parent), penColor(randColor()), layers({new Layer()}), currentLayer(layers[0]) {}

const QColor& Painting::getPenColor() const {
    return penColor;
}

void Painting::setPenColor(const QColor& color) {
    penColor = color;
    emit penColorChanged(penColor);
}

void Painting::addStroke(const Stroke& stroke) {
    currentLayer->addStroke(stroke);
    emit changed();
}

QPicture Painting::getPicture(const QSize& size) const {
    QPicture picture;
    QPainter p(&picture);

    for (const Layer* layer: layers) {
        QImage img = layer->drawImg(size);
        if (currentLayer == layer && currentTool) {
            QPainter imgP(&img);
            currentTool->paint(imgP);
        }
        p.drawImage(QPoint(), img);
    }
    return picture;
}

int Painting::strokesCount() const {
    int size = 0;
    for (const Layer* layer: layers){
        size += layer->strokesCount();
    }
    return size;
}

void Painting::addLayer() {
    layers.append(new Layer());
}

void Painting::selectLayer(int num) {
    currentLayer = layers[num];
}

void Painting::setCurrentTool(Tool* tool) {
    currentTool = tool;
}

Painting::~Painting() {
    for (Layer* layer: layers) {
        delete layer;
    }
}
