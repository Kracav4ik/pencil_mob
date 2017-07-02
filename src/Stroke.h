#pragma once

#include <QPolygon>
#include <QColor>


class QPainter;

//! Stroke for drawing.
struct Stroke{
    //! The color of the stroke.
    QColor color;
    //! If stroke eraser.
    bool isEraser;
    //! List of points of the stroke.
    QPolygon polygon;

    //! Create stroke.
    //! \param color Stroke color
    //! \param isEraser If stroke eraser.
    //! \param polygon List of points of the stroke.
    explicit Stroke(const QColor& color, bool isEraser=false, const QPolygon& polygon=QPolygon());

    //! Draws a polylines from self polygon.
    void paint(QPainter& painter) const;
};



