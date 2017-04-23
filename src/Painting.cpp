#include "Painting.h"
#include <QPainter>
#include "tools/Tool.h"
#include "Stroke.h"
#include "Layer.h"

QColor randColor() {
    return QColor(qrand()%256, qrand()%256, qrand()%256);
}

Painting::Painting(QObject* parent)
        : QObject(parent), penColor(randColor()) {}

const QColor& Painting::getPenColor() const {
    return penColor;
}

void Painting::setPenColor(const QColor& color) {
    penColor = color;
    emit penColorChanged(penColor);
}

void Painting::addStroke(const Stroke& stroke) {
    addStroke(currentLayer, stroke);
}

QPicture Painting::getPicture(const QSize& size) const {
    QPicture picture;
    QPainter p(&picture);

    for (uint32_t i = 0; i < layers.size(); ++i) {
        const Layer* layer = layers[i];
        QImage img = layer->drawImg(size);
        if (currentLayer == i && currentTool) {
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

void Painting::addLayer(const QString& name) {
    layers.append(new Layer(name));
    emit layerAdded((uint32_t) (layers.size() - 1), name);
}

void Painting::selectLayer(uint32_t num) {
    currentLayer = num;
}

void Painting::setCurrentTool(Tool* tool) {
    currentTool = tool;
}

Painting::~Painting() {
    for (Layer* layer: layers) {
        delete layer;
    }
}

uint32_t Painting::getCurrentLayerId() const {
    return currentLayer;
}

void Painting::renameLayer(uint32_t idx, const QString& name) {
    layers[idx]->setName(name);
    emit layerNameChanged(idx, name);
}

const Layer* Painting::getCurrentLayer() const {
    return layers[currentLayer];
}

void Painting::addStroke(uint32_t idx, const Stroke& stroke) {
    layers[idx]->addStroke(stroke);
    emit changed();
}
