#pragma once
#include <QColor>
#include <QPicture>
#include <QVector>

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
    void layerNameChanged(uint32_t idx, const QString& name);
    void layerAdded(uint32_t idx, const QString& name);

public slots:
    void addStroke(const Stroke& stroke);
    void addLayer(const QString& name);
    void selectLayer(uint32_t layerId);
    void setCurrentTool(Tool* tool);

public:
    Painting(QObject* parent);
    ~Painting();

    const QColor& getPenColor() const;
    void setPenColor(const QColor& color);

    QPicture getPicture(const QSize& size) const;

    int strokesCount() const;

    void renameLayer(uint32_t idx, const QString& name);

    const Layer* getCurrentLayer() const;
    uint32_t getCurrentLayerId() const;
};


