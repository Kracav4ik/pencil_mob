#pragma once

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

struct Stroke{
    QColor color;
    QPolygon polygon;

    Stroke(const QColor& color, const QPolygon& polygon=QPolygon());
};

class CanvasWidget : public QWidget {
Q_OBJECT
private:
    QColor penColor;
    bool drawingStroke = false;
    QList<Stroke> strokes;
public:
    CanvasWidget(QWidget *parent);
    const QColor& getPenColor() const;
    void setPenColor(const QColor& color);
    void addStroke(const Stroke& stroke);

signals:
    void debugInfo(int linesCount, int paintTime);
    void strokeFinished(const Stroke& stroke);

protected:
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;
};

