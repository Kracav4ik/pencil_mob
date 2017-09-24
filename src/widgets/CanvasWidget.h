#pragma once

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>

#include "Camera.h"
#include "tools/Tool.h"
#include "Painting.h"


//! This is the main widget on which you can draw with tools.
class CanvasWidget : public QWidget {
Q_OBJECT
private:
    //! This is the painting we are drawing.
    Painting* painting = nullptr;
    //! Camera of the canvas.
    Camera camera;
    //! Current mouse position in screen coordinates.
    QPointF curMousePos;

public slots:
    //! Zoom camera around mouse pos by multiplying scale by s.
    //! \param s value to multiply by (usually you'd want (1+delta) or 1/(1+delta), where delta is close to 0).
    void zoomCamera(float s);
    //! Move camera by specified shift.
    //! \param delta shift that is added to current translation.
    void moveCamera(const QPointF& delta);

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
    //! A signal that is emitted when mouse wheel is rotated.
    //! \param delta value of corresponds to rotation by 15 degrees, which for most mice is one "tick" of rotation.
    void mouseWheel(float delta);
    // TODO: leftDrag and rightDrag are too different, but shouldn't be
    //! A signal that is emitted when mouse is dragged with right button.
    //! \param delta distance the mouse was dragged since previous emit, in screen coordinates.
    void rightDrag(const QPointF& delta);

    //! Ending movement of the mouse.
    void beginDrag();
    //! Beginning movement of the mouse.
    void endDrag();

    //! Continued movement of the mouse.
    //! \param pos The position of the mouse.
    void leftDrag(const QPoint& pos);

protected:
    void paintEvent(QPaintEvent *event) override;

    void wheelEvent(QWheelEvent* event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};
