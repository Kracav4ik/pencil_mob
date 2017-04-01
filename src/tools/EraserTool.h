#pragma once

#include "Tool.h"

class Stroke;

class EraserTool: public Tool {
Q_OBJECT
private:
    Stroke* currentStoke = nullptr;
public:
    void beginDrag() override;

    void drag(const QPoint& pos) override;

    void endDrag() override;

    void paint(QPainter& painter) override;

signals:
    void strokeFinished(const Stroke& stroke);
};
