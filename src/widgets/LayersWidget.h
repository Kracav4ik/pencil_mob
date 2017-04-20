#pragma once

#include <QDockWidget>
#include <QButtonGroup>

#include "ui_layers.h"

class LayersWidget : public QDockWidget, private Ui::LayersWidget{
Q_OBJECT
private:
    QButtonGroup layerButtons;
    friend class ClientMainWindow; // TODO: unfriend
    
public slots:
    void on_renameLayer_clicked();

public:
    LayersWidget(QWidget* parent);

    void appendLayer(QString name=QString());

    int getCountLayers() const;
    QAbstractButton* getCurBut();
    QAbstractButton* getIdxBut(uint32_t idx);
    uint32_t getCurButIdx();
    QAbstractButton* getLastBut();

signals:
    void selectedLayer(uint32_t);
};


