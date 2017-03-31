#pragma once

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

#include "Painting.h"


class CanvasWidget : public QWidget {
Q_OBJECT
private:
    Painting* painting = nullptr;

public:
    CanvasWidget(QWidget *parent);

    void setPainting(Painting* p);

signals:
    void debugInfo(int linesCount, int paintTime);

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;
};

