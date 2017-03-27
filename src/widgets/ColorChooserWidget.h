#pragma once

#include <QDockWidget>

#include "ui_color_chooser.h"

class ColorChooserWidget : public QDockWidget, private Ui::ColorChooserWidget {
Q_OBJECT
private slots:
    void updateColor();
    void on_sliderL_valueChanged(int value);

private:
    QColor color;
    bool isUpdating = false;
    void selectColor(const QColor& c);

public:
    ColorChooserWidget(QWidget* parent);
};


