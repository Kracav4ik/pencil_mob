#pragma once

#include <QDockWidget>

#include "ui_color_chooser.h"

class ColorChooserWidget : public QDockWidget, private Ui::ColorChooserWidget {
Q_OBJECT
private slots:
    void updateColor();
    void on_sliderL_valueChanged(int value);
    void on_rainbowHS_mouseDrag(int x, int y);

private:
    QColor color;
    bool isUpdating = false;

public:
    ColorChooserWidget(QWidget* parent);
    void selectColor(const QColor& c);

signals:
    void colorSelected(const QColor& color);
};


