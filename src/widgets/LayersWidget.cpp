#include "LayersWidget.h"
#include <QInputDialog>

LayersWidget::LayersWidget(QWidget* parent)
        : QDockWidget(parent), layerButtons(this) {
    setupUi(this);
}

void LayersWidget::appendLayer(uint32_t idx, const QString& name) {
    QPushButton* button = new QPushButton(name, this);
    button->setCheckable(true);
    layerButtons.addButton(button, idx);
    if (layerButtons.buttons().size() == 1) {
        button->setChecked(true);
    }

    QVBoxLayout* boxLayout = reinterpret_cast<QVBoxLayout*>(layers->layout());
    boxLayout->insertWidget(boxLayout->count() - 1, button);

    connect(button, &QAbstractButton::toggled, [this, idx](bool checked){
        if (checked) {
            layerButtonClicked(idx);
        }
    });
}

void LayersWidget::changeLayerName(uint32_t idx, const QString& name) {
    layerButtons.button(idx)->setText(name);
}

void LayersWidget::layerButtonClicked(uint32_t idx) {
    emit layerSelected(idx);
}

void LayersWidget::on_renameLayer_clicked() {
    emit renameClicked();
}

void LayersWidget::on_addLayer_clicked() {
    emit addLayerClicked();
}
