#pragma once

#include "Tool.h"

//! Eraser that erases.
class EraserTool: public Tool {
Q_OBJECT
private:
    //! A stroke that erase a eraser. More about stroke in class Stroke
    Stroke* currentStoke = nullptr;
public:
    EraserTool();

    void beginDrag(const QPoint& pos) override;

    void drag(const QPoint& pos) override;

    void endDrag() override;

    void paint(QPainter& painter) override;
};
