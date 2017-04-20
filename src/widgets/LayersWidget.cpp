#include "LayersWidget.h"
#include <QInputDialog>

LayersWidget::LayersWidget(QWidget* parent)
        : QDockWidget(parent), layerButtons(this) {
    setupUi(this);
    appendLayer();
    getLastBut()->click();
}

void LayersWidget::appendLayer(QString name) {
    if (name.isNull()) {
        name = QString("Layer %1").arg(getCountLayers() + 1);
    }
    QPushButton* button = new QPushButton(name, this);
    button->setCheckable(true);
    layerButtons.addButton(button, layerButtons.buttons().size());

    QVBoxLayout* boxLayout = reinterpret_cast<QVBoxLayout*>(layers->layout());
    boxLayout->insertWidget(boxLayout->count() - 1, button);

    connect(button, &QAbstractButton::toggled, [this](bool checked){
        if (checked) {
            uint32_t i = getCurButIdx();
            selectedLayer(i); // TODO WHAT THE ACTUAL FUCK COULD YOU NOT
        }
    });
}

int LayersWidget::getCountLayers() const {
    return layerButtons.buttons().size();
}

QAbstractButton* LayersWidget::getCurBut(){
    return layerButtons.checkedButton();
}

uint32_t LayersWidget::getCurButIdx(){
    return (uint32_t) layerButtons.checkedId();
}

void LayersWidget::on_renameLayer_clicked() {
    // TODO: emit signal
    QString newName = QInputDialog::getText(this, "Choose new name", "Layer name", QLineEdit::Normal, layerButtons.checkedButton()->text());
    if (!newName.isEmpty()) {
        layerButtons.checkedButton()->setText(newName);
    }
}

QAbstractButton* LayersWidget::getLastBut() {
    return layerButtons.buttons()[getCountLayers() - 1];
}

QAbstractButton* LayersWidget::getIdxBut(uint32_t idx) {
    return layerButtons.button(idx);
}
