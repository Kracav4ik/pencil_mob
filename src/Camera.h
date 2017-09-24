#pragma once

#include <QPointF>

class QPainter;

//! Camera for a painter that stores scale and translation.
//! Provides conversions between two coordinate systems:
//!
//! Screen coordinates - pixel coordinates with (0, 0) at the top-left corner of the screen.
//! Distance between (0, 0) and (1, 0) is always one pixel.
//!
//! World coordinates - coordinates with (0, 0) possibly anywhere on the screen.
//! Distance between (0, 0) and (1, 0) depends on the current scale.
class Camera {
private:
    //! Camera scale.
    float scale = 1;
    //! Camera translation.
    QPointF translation;

public:
    //! Returns camera scale.
    float getScale() const;
    //! Sets scale.
    //! \param s new scale.
    void setScale(float s);

    //! Returns camera translation.
    const QPointF& getTranslation() const;
    //! Sets translation.
    //! \param p new translation.
    void setTranslation(const QPointF& p);

    //! Applies camera to the specified painter.
    //! \param p painter.
    void apply(QPainter& p) const;

    //! Transform screen coordinates to world coordinates.
    QPoint screenToWorld(const QPoint& pos) const;
    //! Transform world coordinates to screen coordinates.
    QPoint worldToScreen(const QPoint& pos) const;
};
