#pragma once


#include <QObject>

class QPainter;
class QPoint;
class Stroke;

//! Abstract class for inheritance of tools.
class Tool: public QObject {
Q_OBJECT
protected:
    int brushSize = 5;

public:
    //! Beginning of the mouse drag.
    virtual void beginDrag(const QPoint& pos);
    //! Drag of the mouse.
    //! \param pos mouse position.
    virtual void drag(const QPoint& pos);
    //! Ending of the mouse drag.
    virtual void endDrag();
    //! Drawing with the tool.
    //! \param painter Painter who draw.
    virtual void paint(QPainter& painter);

    virtual void setBrushSize(int bs);
    int getBrushSize();

signals:
    //! It is called if you need to repaint.
    void needRepaint();
    //! Tells painting thats stroke was finished.
    //! \param stroke
    void strokeFinished(const Stroke& stroke);
};
