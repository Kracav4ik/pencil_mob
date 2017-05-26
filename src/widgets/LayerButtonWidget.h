#pragma once

#include <QFrame>

#include "ui_layer_button.h"


class LayerButtonWidget: public QFrame, private Ui::LayerButtonWidget {
Q_OBJECT
private slots:
    void on_upButton_clicked();
    void on_downButton_clicked();

protected:
    void mousePressEvent(QMouseEvent* event) override;

public:
    LayerButtonWidget(QWidget* parent, const QString& layerName);

    void setSelected(bool selected);
    void setName(const QString& layerName);

signals:
    void clicked();
    void upButtonClicked();
    void downButtonClicked();
};
