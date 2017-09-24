#pragma once


#include <QWidget>
#include <functional>

 //! Drawing mechanism.
class CustomPaintWidget: public QWidget{
Q_OBJECT
public:
     //! Creates new widget.
     //! \param parent parent widget.
    explicit CustomPaintWidget(QWidget* parent);

     //! Function type for painting the widget.
    typedef std::function<void(const CustomPaintWidget&, QPainter&)> PaintFunction;

     //! Sets function for painting the widget.
     //! \param p new function for painting the widget.
    void setPaintFunction(const PaintFunction& p);

private:
     //! Function for painting the widget.
    PaintFunction paintFunction;

     //! Check mouse press.
    bool isMousePressed = false;

signals:
     //! Reports that the mouse has moved.
     //! \param x, y Mouse coordinates.
    void mouseDrag(int x, int y);


protected:
    void paintEvent(QPaintEvent *event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};
