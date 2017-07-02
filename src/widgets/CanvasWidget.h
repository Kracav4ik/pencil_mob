#pragma once

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

#include "tools/Tool.h"
#include "Painting.h"


//! This is the main widget on which you can draw with tools.
class CanvasWidget : public QWidget {
Q_OBJECT
private:
    //! This is the painting we are drawing.
    Painting* painting = nullptr;

public:
    //! Create widget.
    //! \param parent parent widget
    explicit CanvasWidget(QWidget *parent);

    //! Sets the painting to the widget.
    //! \param p New painting.
    void setPainting(Painting* p);

signals:
    //! Checks how long it will take to draw, depending on the lines.
    //! \param linesCount Number of lines.
    //! \param paintTime Drawing time.
    void debugInfo(int linesCount, int paintTime);

    //! Ending movement of the mouse.
    void beginDrag();
    //! Beginning movement of the mouse.
    void endDrag();

    //! Continued movement of the mouse.
    //! \param pos The position of the mouse.
    void drag(const QPoint& pos);

protected:
    //! An overloaded function that catches an event, the widget needs to be redrawn.
    //! \param event Qt event for painting
    void paintEvent(QPaintEvent *event) override;

    //! An overloaded function that catches the mouse press event on the widget to start drawing.
    //! \param event Qt event for mouse press
    void mousePressEvent(QMouseEvent *event) override;

    //! An overloaded function that catches the event by releasing the mouse on the widget to stop drawing.
    //! \param event Qt event for mouse release
    void mouseReleaseEvent(QMouseEvent *event) override;

    //! An overloaded function that catches a mouse move on the widget to draw.
    //! \param event Qt event for mouse move
    void mouseMoveEvent(QMouseEvent *event) override;
};

