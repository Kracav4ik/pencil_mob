#include "LayersWidget.h"

#include "LayerButtonWidget.h"
#include <QInputDialog>

LayersWidget::LayersWidget(QWidget* parent)
        : QDockWidget(parent) {
    setupUi(this);
}

void LayersWidget::appendLayer(LayerId uid, const QString& name, bool pre) {
    LayerButtonWidget* button = new LayerButtonWidget(this, QString("%1: %2").arg(uid.user).arg(name));
    uidToOtherLayer[uid] = button;

    addButtonWidget(button, pre);
    button->setEnabled(false);
}

void LayersWidget::addButtonWidget(LayerButtonWidget* button, bool pre) {
    QVBoxLayout* boxLayout = getButtonsLayout();
    if (pre) {
        boxLayout->insertWidget(0, button);
    } else {
        boxLayout->insertWidget(boxLayout->count() - 1, button);
    }
}

void LayersWidget::appendOwnLayer(uint32_t ownUid, const QString& name) {
    LayerButtonWidget* button = new LayerButtonWidget(this, name);
    uidToOwnLayer[ownUid] = button;

    addButtonWidget(button);

    connect(button, &LayerButtonWidget::layerVisibleClicked, [this, ownUid](bool visible) {
        emit layerVisibleChanged(ownUid, visible);
    });

    connect(button, &LayerButtonWidget::clicked, [this, ownUid]() {
        emit layerSelected(ownUid);
    });
    connect(button, &LayerButtonWidget::upButtonClicked, [this, ownUid]() {
        emit upButtonClicked(ownUid);
    });
    connect(button, &LayerButtonWidget::downButtonClicked, [this, ownUid]() {
        emit downButtonClicked(ownUid);
    });
}

void LayersWidget::deleteLayer(LayerId uid) {
    removeButtonWidget(uidToOtherLayer.take(uid));
}

void LayersWidget::deleteOwnLayer(uint32_t uid) {
    removeButtonWidget(uidToOwnLayer.take(uid));
}

void LayersWidget::removeButtonWidget(LayerButtonWidget* button) {
    QVBoxLayout* boxLayout = getButtonsLayout();
    boxLayout->removeWidget(button);
    button->deleteLater();
}

void LayersWidget::selectLayer(uint32_t uid) {
    for (uint32_t buttonUid : uidToOwnLayer.keys()) {
        uidToOwnLayer[buttonUid]->setSelected(buttonUid == uid);
    }
}

void LayersWidget::changeLayerName(uint32_t uid, const QString& name) {
    LayerButtonWidget* button = uidToOwnLayer[uid];
    if (!button) {
        printf("ERRRROR: changeLayerName cannot find button %d\n", uid);
        return;
    }
    button->setName(name);
}

void LayersWidget::changeOwnLayerName(LayerId uid, const QString& name) {
    LayerButtonWidget* button = uidToOtherLayer[uid];
    if (!button) {
        printf("ERRRROR: changeLayerName cannot find button %d\n", uid);
        return;
    }
    button->setName(name);
}

void LayersWidget::on_renameLayer_clicked() {
    emit renameClicked();
}

void LayersWidget::on_addLayer_clicked() {
    emit addLayerClicked();
}

void LayersWidget::on_removeLayer_clicked(){
    emit removeLayerClicked();
}

void LayersWidget::on_duplicateLayer_clicked() {
    emit duplicateLayerClicked();
}

QVBoxLayout* LayersWidget::getButtonsLayout() {
    return reinterpret_cast<QVBoxLayout*>(layers->layout());
}

void LayersWidget::moveOwnLayer(uint32_t uid, uint32_t newPos) {
    LayerButtonWidget* button = uidToOwnLayer[uid];
    if (!button) {
        qDebug() << "ERRRROR: moveLayer cannot find button" << uid;
        return;
    }
    moveButtonWidgetToNewPos(button, newPos);
}

void LayersWidget::moveLayer(LayerId uid, uint32_t newPos) {
    LayerButtonWidget* button = uidToOtherLayer[uid];
    if (!button) {
        qDebug() << "ERRRROR: moveLayer cannot find button" << uid;
        return;
    }
    moveButtonWidgetToNewPos(button, newPos);
}

void LayersWidget::moveButtonWidgetToNewPos(LayerButtonWidget* button, uint32_t newPos) {
    QVBoxLayout* layout = getButtonsLayout();
    layout->removeWidget(button);
    layout->insertWidget(newPos, button);
}
