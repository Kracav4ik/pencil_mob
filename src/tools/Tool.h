#pragma once


#include <QObject>

class QPainter;
class QPoint;

//! Abstract class for inheritance of tools.
class Tool: public QObject {
Q_OBJECT
public:
    //! Delete tool.
    virtual ~Tool();

    //! Beginning of the mouse drag.
    virtual void beginDrag();
    //! Drag of the mouse.
    //! \param pos mouse position.
    virtual void drag(const QPoint& pos);
    //! Ending of the mouse drag.
    virtual void endDrag();
    //! Drawing with the tool.
    //! \param painter Painter who draw.
    virtual void paint(QPainter& painter);

signals:
    //! It is called if you need to repaint.
    void needRepaint();
};
