#pragma once

#include <QDockWidget>
#include <QButtonGroup>

#include "ui_layers.h"

class LayersWidget : public QDockWidget, private Ui::LayersWidget{
Q_OBJECT
private:
    QButtonGroup layerButtons;

    void layerButtonClicked(uint32_t idx);

private slots:
    void on_renameLayer_clicked();
    void on_addLayer_clicked();

public slots:
    void appendLayer(uint32_t idx, const QString& name);
    void changeLayerName(uint32_t idx, const QString& name);

public:
    LayersWidget(QWidget* parent);

signals:
    void layerSelected(uint32_t idx);
    void renameClicked();
    void addLayerClicked();
};
