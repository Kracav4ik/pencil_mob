#pragma once
#include <QColor>
#include <QPicture>
#include <QVector>
#include <QHash>

class Tool;
class Layer;
class Stroke;
class ClientMainWindow;

class Painting : public QObject {
Q_OBJECT
private:
    QColor penColor;

    QHash<uint32_t, Layer*> uidToLayer;
    QVector<uint32_t> zOrder;

    static const uint32_t NO_LAYER = 0;
    uint32_t currentLayer = NO_LAYER;
    uint32_t nextLayerUid = 1000;
    Tool* currentTool = nullptr;

    const Layer* getByUid(uint32_t uid) const;
    Layer* getByUid(uint32_t uid);

signals:
    void changed();
    void penColorChanged(const QColor& color);
    void layerNameChanged(uint32_t uid, const QString& name);
    void layerAdded(uint32_t uid, const QString& name);
    void layerMoved(uint32_t uid, uint32_t newPos);
    void layerRemoved(uint32_t uid);
    void layerSelected(uint32_t uid);
    void layerDuplicated(uint32_t uid);

public slots:
    void addStroke(const Stroke& stroke);
    uint32_t addLayer(const QString& name);
    void selectLayer(uint32_t uid);
    void moveLayer(uint32_t uid, uint32_t newPos);
    void copyFromLayer(uint32_t fromUid, uint32_t toUid);
    void setCurrentTool(Tool* tool);

public:
    explicit Painting(QObject* parent);
    ~Painting();

    void addStroke(uint32_t uid, const Stroke& stroke);

    const QColor& getPenColor() const;
    void setPenColor(const QColor& color);

    QPicture getPicture(const QSize& size) const;

    int strokesCount() const;

    void renameLayer(uint32_t uid, const QString& name);
    void removeLayer(uint32_t uid);

    const Layer* getCurrentLayer() const;
    uint32_t getCurrentLayerId() const;

    int layersCount() const;
    int layerIndex(uint32_t uid) const;
    bool hasLayers() const;
};


