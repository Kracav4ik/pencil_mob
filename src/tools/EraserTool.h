#pragma once

#include "Tool.h"

class Stroke;

//! Eraser that erases.
class EraserTool: public Tool {
Q_OBJECT
private:
    //! A stroke that erase a eraser. More about stroke in class Stroke
    Stroke* currentStoke = nullptr;
public:
    void beginDrag() override;

    void drag(const QPoint& pos) override;

    void endDrag() override;

    void paint(QPainter& painter) override;

signals:
    //! Tells painting thats stroke was finished.
    //! \param stroke The stroke who was finished.
    void strokeFinished(const Stroke& stroke);
};
