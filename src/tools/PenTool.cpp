#include "PenTool.h"

#include "Stroke.h"

void PenTool::beginDrag() {
    currentStoke = new Stroke(penColor);
}

void PenTool::drag(const QPoint& pos) {
    if (currentStoke) {
        currentStoke->polygon << pos;
        emit needRepaint();
    }
}

void PenTool::endDrag() {
    if (currentStoke) {
        emit strokeFinished(*currentStoke);
        delete currentStoke;
        currentStoke = nullptr;
        emit needRepaint();
    }
}

void PenTool::paint(QPainter& painter) {
    if (currentStoke) {
        currentStoke->paint(painter);
    }
}

void PenTool::setPenColor(const QColor& color) {
    penColor = color;
}
