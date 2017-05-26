#pragma once

#include <QDockWidget>
#include <QHash>

#include "ui_layers.h"

class LayerButtonWidget;

class LayersWidget : public QDockWidget, private Ui::LayersWidget{
Q_OBJECT
private:
    QHash<uint32_t, LayerButtonWidget*> uidToLayer;
    QVBoxLayout* getButtonsLayout();

private slots:
    void on_renameLayer_clicked();
    void on_addLayer_clicked();

public slots:
    void appendLayer(uint32_t uid, const QString& name);
    void changeLayerName(uint32_t uid, const QString& name);
    void moveLayer(uint32_t uid, uint32_t newPos);

public:
    explicit LayersWidget(QWidget* parent);

signals:
    void layerSelected(uint32_t uid);
    void renameClicked();
    void addLayerClicked();
    void upButtonClicked(uint32_t uid);
    void downButtonClicked(uint32_t uid);
};
