#pragma once


#include <QWidget>
#include <functional>

class CustomPaintWidget: public QWidget{
Q_OBJECT
public:
    explicit CustomPaintWidget(QWidget* parent);
    typedef std::function<void(const CustomPaintWidget&, QPainter&)> PaintFunction;
    void setPaintFunction(const PaintFunction& p);

private:
    PaintFunction paintFunction;
    bool isMousePressed = false;

signals:
    void mouseDrag(int x, int y);


protected:
    void paintEvent(QPaintEvent *event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};


