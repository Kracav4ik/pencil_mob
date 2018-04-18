#include "Tool.h"
#include <QPoint>
#include <QPainter>

void Tool::beginDrag(const QPoint& pos) {}

void Tool::drag(const QPoint& pos) {}

void Tool::endDrag() {}

void Tool::paint(QPainter& painter) {}

void Tool::setBrushSize(int size) {
    brushSize = size;
}

int Tool::getBrushSize() {
    return brushSize;
}

Tool::Tool(int brushSize):brushSize(brushSize)  {}
