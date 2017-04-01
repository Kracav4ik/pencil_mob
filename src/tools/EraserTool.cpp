#include "EraserTool.h"

#include "Painting.h"

void EraserTool::beginDrag() {
    currentStoke = new Stroke(QColor(), true);
}

void EraserTool::drag(const QPoint& pos) {
    if (currentStoke) {
        currentStoke->polygon << pos;
        emit needRepaint();
    }
}

void EraserTool::endDrag() {
    if (currentStoke) {
        emit strokeFinished(*currentStoke);
        delete currentStoke;
        currentStoke = nullptr;
        emit needRepaint();
    }
}

void EraserTool::paint(QPainter& painter) {
    if (currentStoke) {
        currentStoke->paint(painter);
    }
}
