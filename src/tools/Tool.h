#pragma once


#include <QObject>
#include <QPainter>

class Tool: public QObject {
Q_OBJECT
public:
    virtual ~Tool();

    virtual void beginDrag();
    virtual void drag(const QPoint& pos);
    virtual void endDrag();
    virtual void paint(QPainter& painter);

signals:
    void needRepaint();
};


