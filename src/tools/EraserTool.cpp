#include "EraserTool.h"

#include "Stroke.h"

void EraserTool::beginDrag(const QPoint& pos) {
    currentStoke = new Stroke(QColor(), true, QPolygon{{pos}});
    emit needRepaint();
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
