#pragma once

#include "Serializable.h"

#include <QPolygon>
#include <QColor>
#include <QDebug>


class QPainter;

//! Stroke for drawing.
struct Stroke: public Serializable {
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
    explicit Stroke(const QColor& color=QColor::Invalid, bool isEraser=false, const QPolygon& polygon=QPolygon());

    void read(const QJsonObject& json) override;
    void write(QJsonObject& json) const override;

    //! Draws a polylines from self polygon.
    void paint(QPainter& painter) const;
};

QDebug operator<<(QDebug debug, const Stroke& stroke);
