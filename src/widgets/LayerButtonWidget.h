#pragma once

#include <QFrame>

#include "ui_layer_button.h"


class LayerButtonWidget: public QFrame, private Ui::LayerButtonWidget {
Q_OBJECT
public:
    LayerButtonWidget(QWidget* parent, const QString& layerName);

    void setSelected(bool selected);
private:

};
