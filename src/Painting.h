#pragma once
#include <QPolygon>
#include <QColor>
#include <QPicture>


struct Stroke{
    QColor color;
    bool isEraser;
    QPolygon polygon;

    Stroke(const QColor& color, bool isEraser=false, const QPolygon& polygon=QPolygon());

    void paint(QPainter& painter) const;
};


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


