#pragma once

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

#include "tools/Tool.h"
#include "Painting.h"


class CanvasWidget : public QWidget {
Q_OBJECT
private:
    Painting* painting = nullptr;
    Tool* currentTool = nullptr;

public slots:
    void setCurrentTool(Tool* tool);

public:
    CanvasWidget(QWidget *parent);

    void setPainting(Painting* p);

signals:
    void debugInfo(int linesCount, int paintTime);

    void beginDrag();
    void endDrag();
    void drag(const QPoint& pos);

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;
};

