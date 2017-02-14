#pragma once

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>


class CanvasWidget : public QWidget {
Q_OBJECT
private:
    QColor penColor;
    bool flag = false;
    QList<QPolygon> trajectory;
public:
    CanvasWidget(QWidget *parent);

signals:
    void debugInfo(int linesCount, int paintTime);

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;
};

