#pragma once

#include <QDockWidget>

#include "ui_color_chooser.h"

class ColorChooserWidget : public QDockWidget, private Ui::ColorChooserWidget {
Q_OBJECT
private slots:
    void updateColor(const QString& s);
    void on_sliderL_valueChanged(int value);

private:
    QColor color;
    void selectColor(const QColor& c);

public:
    ColorChooserWidget(QWidget* parent);
};


