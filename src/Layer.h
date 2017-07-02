#pragma once

#include <QImage>
#include "Stroke.h"

//! This is the layer on which we draw with tools.
class Layer {
private:
    //! List of strokes.
    QList<Stroke> strokes;
    //! Layer name.
    QString name;

public:
    //! Gets layer name.
    const QString& getName() const;
    //! Gets layer name.
    //! \param name New layer name.
    void setName(const QString& name);

    //! Draws a picture and returns it.
    //! \param size Size of picture.
    //! \return Painted a picture.
    QImage drawImg(const QSize& size) const;

    //! Gets count of strokes.
    //! \return Length of list of strokes.
    int strokesCount() const;

    //! Adds a stroke to the list of strokes.
    //! \param stroke Stroke who was added.
    void addStroke(const Stroke& stroke);
    //! Create layer.
    //! \param name layer name.
    explicit Layer(const QString& name);

    //! Copies the strokes of another layer.
    //! \param other another layer.
    void copyFrom(const Layer& other);
};
