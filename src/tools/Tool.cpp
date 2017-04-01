#include "Tool.h"
#include <QPoint>
#include <QPainter>

Tool::~Tool() {}

void Tool::beginDrag() {}

void Tool::drag(const QPoint& pos) {}

void Tool::endDrag() {}

void Tool::paint(QPainter& painter) {}
