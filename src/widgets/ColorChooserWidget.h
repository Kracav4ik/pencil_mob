#pragma once

#include <QDockWidget>

#include "ui_color_chooser.h"

class ColorChooserWidget : public QDockWidget, private Ui::ColorChooserWidget {
Q_OBJECT
private:

public:
    ColorChooserWidget(QWidget* parent);
};


