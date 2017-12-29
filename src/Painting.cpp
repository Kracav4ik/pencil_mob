#include "Painting.h"
#include "tools/Tool.h"
#include "Stroke.h"
#include "Layer.h"
#include <QPainter>
#include <QJsonObject>
#include <QJsonArray>

const uint32_t Painting::NO_LAYER = 0;

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

void Painting::addOwnStroke(const Stroke& stroke) {
    if (!hasOwnLayers()) {
        qDebug() << "Error: addOwnStroke() called for painting with no own layers";
        return; // TODO this should be error; tools should be disabled
    }
    addStroke(getCurrentLayerId(), stroke);
}

void Painting::drawPicture(QPainter& p, const QSize& size, const QPointF& translation) const {
    for (LayerId uid : zOrder) {
        const Layer* layer = getByUid(uid);
        if (!userToVisible[uidFromLayer(layer).user]) {
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

uint32_t Painting::getNextLayerUid() {
    uint32_t uid = nextLayerUid++;
    while (uidToLayer.contains({getOurUserId(), uid})) {
        uid++;
    }
    return uid;
}
LayerId Painting::addLayer(const QString& name) {
    LayerId layer(getOurUserId(), getNextLayerUid());
    addLayer(layer, name);
    return layer;
}

void Painting::addLayer(LayerId idx, const QString& name) {
    uidToLayer[idx] = new Layer(name);
    if (!containsUser(idx.user)) {
        userToVisible[idx.user] = true;
        emit userAdded(idx.user);
    }
    zOrder.append(idx);
    if (idx.user == getOurUserId()) {
        emit ownLayerAdded(idx.layer, name);
    } else {
        emit layerAdded(idx, name);
    }
    if (currentLayer.layer == NO_LAYER) {
        selectLayer(getTopOwnLayer());
    }
}

void Painting::selectLayer(uint32_t uid) {
    LayerId layerId(getOurUserId(), uid);
    if (layerId.layer != NO_LAYER) {
        if (!zOrder.contains(layerId)) {
            qDebug() << "selectLayer: layer to select" << layerId.layer << "in user" << layerId.user << "not found in zOrder list";
            return;
        }
        if (!uidToLayer.contains(layerId)) {
            qDebug() << "selectLayer: layer to select" << layerId.layer << "in user" << layerId.user << "not found in uidToLayer map";
            return;
        }
    }
    currentLayer = layerId;
    emit layerSelected(currentLayer.layer);
}

void Painting::removeLayer(LayerId uid) {
    int currentPos = layerIndex(uid);
    if (currentPos == -1) {
        qDebug() << "removeLayer: currentLayer" << uid.layer << "in user" << uid.user << "not found in zOrder list";
        return;
    }
    if (uid == getCurrentLayerId()) {
        selectLayer(getTopOwnLayer(uid.layer));
    }
    zOrder.removeOne(uid);
    Layer* layer = uidToLayer.take(uid);
    if (!layer) {
        qDebug() << "removeLayer: currentLayer" << uid.layer << "in user" << uid.user << "not found in uidToLayer map";
        return;
    }
    delete layer;

    if (uid.user == getOurUserId()) {
        emit ownLayerRemoved(uid.layer);
    } else {
        emit layerRemoved(uid);
    }
    emit changed();
}

void Painting::moveLayer(LayerId uid, uint32_t newPos) {
    if (newPos >= zOrder.size()) {
        qDebug() << "moveLayer: newPos" << newPos << "is too big for zOrder list of size" << zOrder.size();
        return;
    }
    if (!zOrder.removeOne(uid)) {
        qDebug() << "moveLayer: layer" << uid.layer << "in user" << uid.user << "not found in zOrder list";
        return;
    }
    zOrder.insert(newPos, uid);
    emit layerMoved(uid.layer, newPos);
    emit changed();
}

void Painting::copyFromLayer(LayerId fromUid, LayerId toUid) {
    Layer* fromLayer = getByUid(fromUid);
    if (!fromLayer) {
        qDebug() << "copyFromLayer: target layer" << fromUid.layer << "in user" << fromUid.user << "not found in uidToLayer map";
        return;
    }
    Layer* toLayer = getByUid(toUid);
    if (!toLayer) {
        qDebug() << "renameLayer: copyFromLayer: source layer" << toUid.layer << "in user" << toUid.user << "not found in uidToLayer map";

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

LayerId Painting::getCurrentLayerId() const {
    if (currentLayer.layer == NO_LAYER) {
        qDebug() << "getCurrentLayerId: currentLayer is zero, you are going to crash~ <3";
    }
    return currentLayer;
}

void Painting::renameLayer(LayerId uid, const QString& name) {
    Layer* layer = getByUid(uid);
    if (!layer) {
        qDebug() << "renameLayer: Layer not found for uid" << uid.layer << "in user" << uid.user;
        return;
    }
    layer->setName(name);
    emit layerNameChanged(uid.layer, name);
}

const Layer* Painting::getCurrentLayer() const {
    return getByUid(getCurrentLayerId());
}

Layer* Painting::getCurrentLayer() {
    return getByUid(getCurrentLayerId());
}

QString Painting::getLayerNameFromUid(LayerId uid) {
    return getByUid(uid)->getName();
}

void Painting::addStroke(LayerId uid, const Stroke& stroke) {
    Layer* layer = getByUid(uid);
    if (!layer) {
        qDebug() << "addStroke: Layer not found for uid" << uid.layer << "in user" << uid.user;
        return;
    }
    layer->addStroke(stroke);
    emit changed();
}

void Painting::removeLastStroke(LayerId uid) {
    Layer* layer = getByUid(uid);
    if (!layer) {
        qDebug() << "addStroke: Layer not found for uid" << uid.layer << "in user" << uid.user;
        return;
    }
    layer->removeLastStroke();
    emit changed();
}

const Layer* Painting::getByUid(LayerId uid) const {
    return uidToLayer[uid];
}

Layer* Painting::getByUid(LayerId uid) {
    return uidToLayer[uid];
}

int Painting::layersCount() const {
    return uidToLayer.size();
}

int Painting::layerIndex(LayerId uid) const {
    return zOrder.indexOf(uid);
}

bool Painting::hasOwnLayers() const {
    for (LayerId layer : zOrder) {
        if (layer.user == getOurUserId()) {
            return true;
        }
    }
    return false;
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

bool Painting::containsLayer(LayerId uid) {
    return uidToLayer.contains(uid);
}

LayerId Painting::uidFromLayer(const Layer* layer) const {
    return uidToLayer.key(const_cast<Layer*>(layer), {getOurUserId(), NO_LAYER});
}

void Painting::setOurUserId(uint32_t newUserId) {
    for (LayerId key : uidToLayer.keys()) {
        if (key.user == getOurUserId()) {
            Layer* layer = uidToLayer.take(key);
            uidToLayer[{newUserId, key.layer}] = layer;
        }
    }
    for (uint32_t key : userToVisible.keys()) {
        if (key == getOurUserId()) {
            bool visible = userToVisible.take(key);
            userToVisible[newUserId] = visible;
        }
    }
    for (LayerId& layer : zOrder) {
        if (layer.user == getOurUserId()) {
            layer.user = newUserId;
        }
    }
    ourUserId = newUserId;
    currentLayer.user = newUserId;
//    emit layerSelected(currentLayer.layer);
    emit changed();
}

uint32_t Painting::getTopOwnLayer(uint32_t ignoreLayer) {
    for (auto it = zOrder.rbegin(); it != zOrder.rend(); ++it) {
        if (it->user == getOurUserId() && it->layer != ignoreLayer) {
            return it->layer;
        }
    }
    return NO_LAYER;
}

uint32_t Painting::getOurUserId() const {
    return ourUserId;
}

void Painting::read(const QJsonObject& json) {
    int version = json["version"].toInt(-1);
    if (version == -1) {
        printf("File with invalid version!\n");
    } else {
        printf("Reading from version %d\n", version);
    }

    for (QJsonValue layerJson : json["layers"].toArray()) {
        Layer layer;
        layer.read(layerJson.toObject());
        LayerId uid = {getOurUserId(), getNextLayerUid()};
        addLayer(uid, layer.getName());
        getByUid(uid)->copyFrom(layer);
    }
    emit changed();
}

void Painting::write(QJsonObject& json) const {
    json["version"] = 1;

    QJsonArray newLayers;
    for (const Layer* layer : getLayers()) {
        QJsonObject object;
        layer->write(object);
        newLayers << object;
    }
    json["layers"] = newLayers;
}
