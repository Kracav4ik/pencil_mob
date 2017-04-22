#pragma once

#include "tools/Tool.h"
#include <QColor>

class Stroke;

class PenTool: public Tool {
Q_OBJECT
private:
    QColor penColor;
    Stroke* currentStoke = nullptr;

public:
    void beginDrag() override;
    void drag(const QPoint& pos) override;
    void endDrag() override;

    void paint(QPainter& painter) override;

public slots:
    void setPenColor(const QColor& color);

signals:
    void strokeFinished(const Stroke& stroke);
};
