#pragma once
#include <QColor>
#include <QPicture>
#include "Stroke.h"

class Tool;
class Layer;
class Stroke;
class ClientMainWindow;

class Painting : public QObject {
Q_OBJECT
private:
    QColor penColor;
    QVector<Layer*> layers;
    uint32_t currentLayer = 0;
    Tool* currentTool = nullptr;

signals:
    void changed();
    void penColorChanged(const QColor& color);

public slots:
    void addStroke(const Stroke& stroke);
    void addLayer();
    void selectLayer(uint32_t layerId);
    void setCurrentTool(Tool* tool);

public:
    Painting(QObject* parent);
    ~Painting();

    const QColor& getPenColor() const;
    void setPenColor(const QColor& color);

    QPicture getPicture(const QSize& size) const;

    int strokesCount() const;

    uint32_t getCurrentLayerId() const;
};


