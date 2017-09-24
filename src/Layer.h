#pragma once

#include <QImage>
#include "Stroke.h"

//! This is the layer on which we draw with tools.
class Layer {
private:
    //! List of strokes, see class Stroke for details.
    QList<Stroke> strokes;
    //! Layer name.
    QString name;

public:
    //! Gets layer name.
    const QString& getName() const;
    //! Sets new layer name.
    //! \param name New layer name.
    void setName(const QString& name);

    //! Draws a picture and returns it.
    //! \param size Size of picture.
    //! \param translation Translation to apply during painting.
    //! \return The painted picture.
    QImage drawImg(const QSize& size, const QPointF& translation) const;

    //! Gets count of strokes.
    //! \return Length of list of strokes.
    int strokesCount() const;

    //! Adds a stroke to the list of strokes.
    //! \param stroke Stroke that was added.
    void addStroke(const Stroke& stroke);

    //! Creates layer.
    //! \param name layer name.
    explicit Layer(const QString& name);

    //! Copies the strokes of another layer.
    //! \param other another layer.
    void copyFrom(const Layer& other);
};
