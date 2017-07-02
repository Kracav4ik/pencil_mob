#pragma once

#include <QDockWidget>

#include "ui_color_chooser.h"

//! This is a widget that allows you to choose the color that you want to draw.
class ColorChooserWidget : public QDockWidget, private Ui::ColorChooserWidget {
Q_OBJECT
private slots:
    //! Updates the current color if needed.
    void updateColor();

    //! Changes the color value.
    //! \param value color value
    void on_sliderL_valueChanged(int value);

    //! Selects a color from the palette.
    //! \param x, y Coordinates of the palette.
    void on_rainbowHS_mouseDrag(int x, int y);

private:
    QColor color;
    bool isUpdating = false;

public:
    //! Create widget.
    //! \param parent parent widget
    explicit ColorChooserWidget(QWidget* parent);

    //! Sets a color.
    //! \param c Color.
    void selectColor(const QColor& c);

signals:
    //! A signal that tells the client the selected color.
    //! \param color selected color.
    void colorSelected(const QColor& color);
};
