#include "LayersWidget.h"

#include "LayerButtonWidget.h"
#include <QInputDialog>

LayersWidget::LayersWidget(QWidget* parent)
        : QDockWidget(parent) {
    setupUi(this);
}

void LayersWidget::appendLayer(uint32_t uid, const QString& name) {
    LayerButtonWidget* button = new LayerButtonWidget(this, name);
    uidToLayer[uid] = button;
    if (uidToLayer.size() == 1) {
        button->setSelected(true);
    }

    QVBoxLayout* boxLayout = getButtonsLayout();
    boxLayout->insertWidget(boxLayout->count() - 1, button);

    connect(button, &LayerButtonWidget::clicked, [this, uid]() {
        for (uint32_t buttonUid : uidToLayer.keys()) {
            uidToLayer[buttonUid]->setSelected(buttonUid == uid);
        }
        emit layerSelected(uid);
    });
    connect(button, &LayerButtonWidget::upButtonClicked, [this, uid]() {
        emit upButtonClicked(uid);
    });
    connect(button, &LayerButtonWidget::downButtonClicked, [this, uid]() {
        emit downButtonClicked(uid);
    });
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

QVBoxLayout* LayersWidget::getButtonsLayout() {
    return reinterpret_cast<QVBoxLayout*>(layers->layout());
}

void LayersWidget::moveLayer(uint32_t uid, uint32_t newPos) {
    QVBoxLayout* layout = getButtonsLayout();
    LayerButtonWidget* button = uidToLayer[uid];
    if (!button) {
        printf("ERRRROR: moveLayer cannot find button %d\n", uid);
        return;
    }

    layout->removeWidget(button);
    layout->insertWidget(newPos, button);
}
