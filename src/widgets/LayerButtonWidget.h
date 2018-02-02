#pragma once

#include <QFrame>

#include "ui_layer_button.h"


//! Widget-layer in the list of widgets-layers. You can move it with the button up and down.
class LayerButtonWidget: public QFrame, private Ui::LayerButtonWidget {
Q_OBJECT
private slots:
     //! Click the button up, which moves the layer up.
    void on_upButton_clicked();

     //! Click the button up, which moves the layer up.
    void on_downButton_clicked();

    void on_layerVisible_clicked(bool visible);

protected:
    void mousePressEvent(QMouseEvent* event) override;

public:
     //! Create widget.
     //! \param parent parent widget.
     //! \param layerName name of layer.
    LayerButtonWidget(QWidget* parent, const QString& layerName);

     //! Sets selection layer
     //! \param selected marks layer as current
    void setSelected(bool selected);

     //! Change layer name
     //! \param layerName new layer name
    void setName(const QString& layerName);

signals:
     //! The signal for selected. If you click on widget.
    void clicked();

    void layerVisibleClicked(bool visible);

     //! The signal indicates that the button is up clicked.
    void upButtonClicked();

     //! The signal indicates that the button is down clicked.
    void downButtonClicked();
};
