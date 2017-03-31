#pragma once
#include <QPolygon>
#include <QColor>
#include <QPicture>


struct Stroke{
    QColor color;
    QPolygon polygon;

    Stroke(const QColor& color, const QPolygon& polygon=QPolygon());
};


class Painting : public QObject {
Q_OBJECT
private:
    QColor penColor;
    QList<Stroke> strokes;

signals:
    void changed();
    void strokeFinished(const Stroke& stroke);

public:
    Painting(QObject* parent);

    void addStroke(const Stroke& stroke);

    const QColor& getPenColor() const;
    void setPenColor(const QColor& color);

    QPicture getPicture() const;

    void beginStroke();
    void continueStroke(const QPoint& point);
    void endStroke();

    int strokesSize() const;
};


