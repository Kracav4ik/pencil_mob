#include "LayerButtonWidget.h"

LayerButtonWidget::LayerButtonWidget(QWidget* parent, const QString& layerName) : QFrame(parent) {
    setupUi(this);
    layerNameLabel->setText(layerName);
}

void LayerButtonWidget::setSelected(bool selected) {
    setFrameShadow(selected ? Sunken : Plain);
}
