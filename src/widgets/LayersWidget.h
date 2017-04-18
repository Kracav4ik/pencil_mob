#pragma once

#include <QDockWidget>
#include <QButtonGroup>

#include "ui_layers.h"

class LayersWidget : public QDockWidget, private Ui::LayersWidget{
Q_OBJECT
private:
    QButtonGroup layerButtons;
    friend class ClientMainWindow; // TODO: unfriend
    
public:
    LayersWidget(QWidget* parent);

    void mmm(QVBoxLayout* layout, const char* string);

signals:
    void selectedLayer(uint32_t);
};


