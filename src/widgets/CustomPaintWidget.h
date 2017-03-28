#pragma once


#include <QWidget>
#include <functional>

class CustomPaintWidget: public QWidget{
public:
    CustomPaintWidget(QWidget* parent);
    typedef std::function<void(const CustomPaintWidget&, QPainter&)> PaintFunction;
    void setPaintFunction(const PaintFunction& p);

private:
    PaintFunction paintFunction;

protected:
    void paintEvent(QPaintEvent *event) override;
};


