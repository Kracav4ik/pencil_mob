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
     //! An overloaded function that catches an event, the widget needs to be redrawn.
     //! \param event Qt event for painting
    void paintEvent(QPaintEvent *event) override;

    //! An overloaded function that catches the event by releasing the mouse on the widget to stop drawing.
    //! \param event Qt event for mouse release
    void mouseReleaseEvent(QMouseEvent* event) override;

    //! An overloaded function that catches the mouse press event on the widget to start drawing.
    //! \param event Qt event for mouse press
    void mousePressEvent(QMouseEvent *event) override;

    //! An overloaded function that catches a mouse move on the widget to draw.
    //! \param event Qt event for mouse move.
    void mouseMoveEvent(QMouseEvent *event) override;
};


