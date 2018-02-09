#pragma once

#include "LayerId.h"

#include <QDockWidget>
#include <QHash>

#include "ui_layers.h"

class LayerButtonWidget;

//! A class for storage layers
class LayersWidget : public QDockWidget, private Ui::LayersWidget{
Q_OBJECT
private:
    //! Map from own layers uids to layers objects.
    QHash<uint32_t, LayerButtonWidget*> uidToOwnLayer;
    //! Map from other layers uids to layers objects.
    QHash<LayerId, LayerButtonWidget*> uidToOtherLayer;

     //! Get layout.
     //! @return Layout to add layers.
    QVBoxLayout* getButtonsLayout();

    void removeButtonWidget(LayerButtonWidget* button);
    void moveButtonWidgetToNewPos(LayerButtonWidget* button, uint32_t newPos);
    void addButtonWidget(LayerButtonWidget* button, bool pre=false);

private slots:
     //! It is called when the button rename is clicked.
    void on_renameLayer_clicked();

     //! It is called when the button add is clicked.
    void on_addLayer_clicked();

     //! It is called when the button remove is clicked.
    void on_removeLayer_clicked();

     //! It is called when the button duplicate is clicked.
    void on_duplicateLayer_clicked();

public slots:
     //! Adds layer of another client to list.
     //! \param uid layer uid.
     //! \param name layer name.
    void appendLayer(LayerId uid, const QString& name, bool pre);

     //! Adds our layer to list.
     //! \param uid layer uid.
     //! \param name layer name.
    void appendOwnLayer(uint32_t ownUid, const QString& name);

     //! Deletes layer of another client from the list.
     //! \param uid layer uid.
    void deleteLayer(LayerId uid);

     //! Deletes our layer from the list.
     //! \param uid layer uid.
    void deleteOwnLayer(uint32_t uid);

     //! Selects layer from list own layers.
     //! \param uid layer uid.
    void selectLayer(uint32_t uid);

     //! Changes the name of the layer.
     //! \param uid layer uid.
     //! \param name new layer name.
    void changeLayerName(uint32_t uid, const QString& name);

    void changeOwnLayerName(LayerId uid, const QString& name);

     //! Move a layer of another client from layer uid to new layer's uid using the insert.
     //! \param uid old layer uid.
     //! \param newPos new layer uid.
    void moveLayer(LayerId uid, uint32_t newPos);

     //! Move a layer from layer uid to new layer's uid using the insert.
     //! \param uid old layer uid.
     //! \param newPos new layer uid.
    void moveOwnLayer(uint32_t uid, uint32_t newPos);

public:
     //! Create widget.
     //! \param parent parent widget.
    explicit LayersWidget(QWidget* parent);

signals:
     //! Informs the client that the layer is selected.
     //! \param uid uid of the selected layer.
    void layerSelected(uint32_t uid);

     //! Informs the client that rename layer is clicked.
    void renameClicked();

     //! Informs the client that add layer is clicked.
    void addLayerClicked();

     //! Informs the client that remove layer is clicked.
    void removeLayerClicked();

     //! Informs the client that duplicate layer is clicked.
    void duplicateLayerClicked();

     //! Informs the client that up button is clicked.
    void upButtonClicked(uint32_t uid);

     //! Informs the client that down button is clicked.
    void downButtonClicked(uint32_t uid);

    //! Informs the client that layer is changed visible.
    void layerVisibleChanged(uint32_t uid, bool visible);
};
