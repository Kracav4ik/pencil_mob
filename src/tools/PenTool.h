#pragma once

#include "tools/Tool.h"
#include <QColor>

//! Pen. The main tool for drawing.
class PenTool: public Tool {
Q_OBJECT
private:
    //! The color that the pen draws.
    QColor penColor;
    //! A stroke that draws a pen. More about stroke in class Stroke
    Stroke* currentStoke = nullptr;

public:
    void beginDrag(const QPoint& pos) override;
    void drag(const QPoint& pos) override;
    void endDrag() override;

    void paint(QPainter& painter) override;

public slots:
    //! Sets pen color.
    //! \param color New color that will be set.
    void setPenColor(const QColor& color);
};
