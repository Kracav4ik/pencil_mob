#include "LayerButtonWidget.h"

#include <QMouseEvent>

LayerButtonWidget::LayerButtonWidget(QWidget* parent, const QString& layerName) : QFrame(parent) {
    setupUi(this);
    layerNameLabel->setText(layerName);
}

void LayerButtonWidget::setSelected(bool selected) {
    setFrameShape(selected ? Panel : NoFrame);
}

void LayerButtonWidget::setName(const QString& layerName) {
    layerNameLabel->setText(layerName);
}

void LayerButtonWidget::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton){
        emit clicked();
    }
}

void LayerButtonWidget::on_upButton_clicked() {
    emit upButtonClicked();
}

void LayerButtonWidget::on_downButton_clicked() {
    emit downButtonClicked();
}

void LayerButtonWidget::on_layerVisible_clicked(bool visible) {
    emit layerVisibleClicked(visible);
}
