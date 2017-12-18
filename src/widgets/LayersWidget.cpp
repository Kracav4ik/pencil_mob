#include "LayersWidget.h"

#include "LayerButtonWidget.h"
#include <QInputDialog>

LayersWidget::LayersWidget(QWidget* parent)
        : QDockWidget(parent) {
    setupUi(this);
}

void LayersWidget::appendLayer(LayerId uid, const QString& name) {
    LayerButtonWidget* button = new LayerButtonWidget(this, QString("%1: %2").arg(uid.user).arg(name));

    QVBoxLayout* boxLayout = getButtonsLayout();
    boxLayout->insertWidget(boxLayout->count() - 1, button);
    button->setEnabled(false);
}

void LayersWidget::appendOwnLayer(uint32_t ownUid, const QString& name) {
    LayerButtonWidget* button = new LayerButtonWidget(this, name);
    uidToLayer[ownUid] = button;

    QVBoxLayout* boxLayout = getButtonsLayout();
    boxLayout->insertWidget(boxLayout->count() - 1, button);

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

void LayersWidget::deleteLayer(uint32_t uid) {
    QVBoxLayout* boxLayout = getButtonsLayout();
    LayerButtonWidget* button = uidToLayer.take(uid);
    boxLayout->removeWidget(button);
    button->deleteLater();
}

void LayersWidget::selectLayer(uint32_t uid) {
    for (uint32_t buttonUid : uidToLayer.keys()) {
        uidToLayer[buttonUid]->setSelected(buttonUid == uid);
    }
}

void LayersWidget::changeLayerName(uint32_t uid, const QString& name) {
    LayerButtonWidget* button = uidToLayer[uid];
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

void LayersWidget::moveLayer(uint32_t uid, uint32_t newPos) {
    QVBoxLayout* layout = getButtonsLayout();
    LayerButtonWidget* button = uidToLayer[uid];
    if (!button) {
        qDebug() << "ERRRROR: moveLayer cannot find button" << uid;
        return;
    }

    layout->removeWidget(button);
    layout->insertWidget(newPos, button);
}
