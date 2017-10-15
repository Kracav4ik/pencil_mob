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
    if (!hasLayers()) {
        return; // TODO this should be error; tools should be disabled
    }
    addStroke(getCurrentLayerId(), stroke);
}

void Painting::drawPicture(QPainter& p, const QSize& size, const QPointF& translation) const {
    for (uint32_t uid : zOrder) {
        const Layer* layer = getByUid(uid);
        if (!userToVisible[layer->getUser()]) {
            continue;
        }
        QImage img = layer->drawImg(size, translation);
        if (getCurrentLayerId() == uid && currentTool) {
            QPainter imgP(&img);
            imgP.translate(translation);
            currentTool->paint(imgP);
        }
        p.drawImage(QPoint(), img);
    }
}

int Painting::strokesCount() const {
    int size = 0;
    for (const Layer* layer: uidToLayer){
        size += layer->strokesCount();
    }
    return size;
}

uint32_t Painting::addLayer(uint32_t user, const QString& name) {
    uint32_t uid = nextLayerUid++; 
    uidToLayer[uid] = new Layer(user, name);
    if (!containsUser(user)) {
        userToVisible[user] = true;
    }
    zOrder.append(uid);
    emit layerAdded(uid, name);
    if (uidToLayer.size() == 1) {
        selectLayer(uid);
    }
    return uid;
}

void Painting::selectLayer(uint32_t uid) {
    if (uid != NO_LAYER) {
        if (!zOrder.contains(uid)) {
            printf("selectLayer: layer to select %d not found in zOrder list\n", uid);
            return;
        }
        if (!uidToLayer.contains(uid)) {
            printf("selectLayer: layer to select %d not found in uidToLayer map\n", uid);
            return;
        }
    }
    currentLayer = uid;
    emit layerSelected(currentLayer);
}

void Painting::removeLayer(uint32_t uid) {
    int currentPos = zOrder.indexOf(uid);
    if (currentPos == -1) {
        printf("removeLayer: currentLayer %d not found in zOrder list\n", uid);
        return;
    }
    zOrder.removeOne(uid);
    Layer* layer = uidToLayer.take(uid);
    if (!layer) {
        printf("removeLayer: currentLayer %d not found in uidToLayer map\n", uid);
        return;
    }
    delete layer;

    if (uid == getCurrentLayerId()) {
        if (zOrder.empty()) {
            selectLayer(NO_LAYER);
        } else {
            selectLayer(zOrder[qMin(currentPos, zOrder.size() - 1)]);
        }
    }
    emit layerRemoved(uid);
    emit changed();
}

void Painting::moveLayer(uint32_t uid, uint32_t newPos) {
    if (newPos >= zOrder.size()) {
        printf("moveLayer: newPos %d is too big for zOrder list of size %d\n", newPos, zOrder.size());
        return;
    }
    if (!zOrder.removeOne(uid)) {
        printf("moveLayer: layer %d not found in zOrder list\n", uid);
        return;
    }
    zOrder.insert(newPos, uid);
    emit layerMoved(uid, newPos);
    emit changed();
}

void Painting::copyFromLayer(uint32_t fromUid, uint32_t toUid) {
    Layer* fromLayer = getByUid(fromUid);
    if (!fromLayer) {
        printf("copyFromLayer: target layer %d not found in uidToLayer map\n", fromUid);
        return;
    }
    Layer* toLayer = getByUid(toUid);
    if (!toLayer) {
        printf("copyFromLayer: source layer %d not found in uidToLayer map\n", toUid);
        return;
    }
    toLayer->copyFrom(*fromLayer);
    emit changed();
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
    if (currentLayer == NO_LAYER) {
        printf("getCurrentLayerId: currentLayer is zero, you are going to crash~ <3\n");
    }
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
    return getByUid(getCurrentLayerId());
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

int Painting::layersCount() const {
    return uidToLayer.size();
}

int Painting::layerIndex(uint32_t uid) const {
    return zOrder.indexOf(uid);
}

bool Painting::hasLayers() const {
    return layersCount() > 0;
}

void Painting::changingUserVisible(uint32_t user, bool visible) {
    userToVisible[user] = visible;
    emit changed();
}

bool Painting::containsUser(uint32_t user) {
    return userToVisible.contains(user);
}

QList<Layer*> Painting::getLayers() const {
    return uidToLayer.values();
}

bool Painting::containsLayer(uint32_t user, const QString& name) {
    for (Layer* layer : uidToLayer.values()) {
        if (layer->getUser() == user && layer->getName() == name) {
            return true;
        }
    }
    return false;
}
