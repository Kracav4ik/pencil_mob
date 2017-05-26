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

    for (uint32_t uid : zOrder) {
        const Layer* layer = getByUid(uid);
        QImage img = layer->drawImg(size);
        if (currentLayer == uid && currentTool) {
            QPainter imgP(&img);
            currentTool->paint(imgP);
        }
        p.drawImage(QPoint(), img);
    }
    return picture;
}

int Painting::strokesCount() const {
    int size = 0;
    for (const Layer* layer: uidToLayer){
        size += layer->strokesCount();
    }
    return size;
}

void Painting::addLayer(const QString& name) {
    uint32_t uid = nextLayerUid++; 
    uidToLayer[uid] = new Layer(name);
    zOrder.append(uid);
    emit layerAdded(uid, name);
}

void Painting::selectLayer(uint32_t uid) {
    currentLayer = uid;
}

void Painting::moveLayerDown(uint32_t uid) {
    int index = zOrder.indexOf(uid);
    if (index == -1) {
        printf("moveLayerDown: Layer not found for uid %d\n", uid);
        return;
    }
    if (index < zOrder.size() - 1) {
        qSwap(zOrder[index], zOrder[index + 1]);
        emit layerMoved(uid, (uint32_t) (index + 1));
        emit changed();
    }
}

void Painting::moveLayerUp(uint32_t uid) {
    int index = zOrder.indexOf(uid);
    if (index == -1) {
        printf("moveLayerUp: Layer not found for uid %d\n", uid);
        return;
    }
    if (index > 0) {
        qSwap(zOrder[index], zOrder[index - 1]);
        emit layerMoved(uid, (uint32_t) (index - 1));
        emit changed();
    }
}

void Painting::setCurrentTool(Tool* tool) {
    currentTool = tool;
}

Painting::~Painting() {
    for (Layer* layer: uidToLayer) {
        delete layer;
    }
}

uint32_t Painting::getCurrentLayerId() const {
    return currentLayer;
}

void Painting::renameLayer(uint32_t uid, const QString& name) {
    Layer* layer = getByUid(uid);
    if (!layer) {
        printf("renameLayer: Layer not found for uid %d\n", uid);
        return;
    }
    layer->setName(name);
    emit layerNameChanged(uid, name);
}

const Layer* Painting::getCurrentLayer() const {
    return getByUid(currentLayer);
}

void Painting::addStroke(uint32_t uid, const Stroke& stroke) {
    Layer* layer = getByUid(uid);
    if (!layer) {
        printf("addStroke: Layer not found for uid %d\n", uid);
        return;
    }
    layer->addStroke(stroke);
    emit changed();
}

const Layer* Painting::getByUid(uint32_t uid) const {
    return uidToLayer[uid];
}

Layer* Painting::getByUid(uint32_t uid) {
    return uidToLayer[uid];
}
