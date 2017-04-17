#pragma once
#include <QColor>
#include <QPicture>
#include "Stroke.h"


class Painting : public QObject {
Q_OBJECT
private:
    QColor penColor;
    QList<Stroke> strokes;

signals:
    void changed();
    void penColorChanged(const QColor& color);

public slots:
    void addStroke(const Stroke& stroke);

public:
    Painting(QObject* parent);

    const QColor& getPenColor() const;
    void setPenColor(const QColor& color);

    QPicture getPicture() const;

    int strokesCount() const;
};


