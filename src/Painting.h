#pragma once
#include <QColor>
#include <QPicture>
#include <QVector>
#include <QHash>

class Tool;
class Layer;
class Stroke;
class ClientMainWindow;

//! The main part is where all the drawing takes place.
class Painting : public QObject {
Q_OBJECT
private:
    //! Color of pen which we're drawing.
    QColor penColor;

    //! Map from uid layer to layer.
    QHash<uint32_t, Layer*> uidToLayer;
    //! Layers z-order.
    QVector<uint32_t> zOrder;

    //! Constant when we did not choose a current layer.
    static const uint32_t NO_LAYER = 0;
    //! Layer on which we're drawing.
    uint32_t currentLayer = NO_LAYER;
    //! The next one layer uid if we add or duplicate the layer.
    uint32_t nextLayerUid = 1000;
    //! Tool which we're drawing.
    Tool* currentTool = nullptr;

    //! Returned layer from layer uid.
    //! \param uid layer uid
    //! \return Layer.
    const Layer* getByUid(uint32_t uid) const;
    //! Returned layer from layer uid.(non-const)
    //! \param uid layer uid
    //! \return Layer.
    Layer* getByUid(uint32_t uid);

signals:
    //! Emitted when the painting is changed and need repaint
    void changed();

    //! Emitted when the painting color is changed.
    //! \param color New color.
    void penColorChanged(const QColor& color);

    //! Emitted when the layer name is changed.
    //! \param uid Layer uid.
    //! \param name New layer name.
    void layerNameChanged(uint32_t uid, const QString& name);

    //! Emitted when the new layer is added.
    //! \param uid New uid layer.
    //! \param name New layer name.
    void layerAdded(uint32_t uid, const QString& name);

    //! Emitted when the layer is moved.
    //! \param uid Layer uid.
    //! \param newPos Z-order position.
    void layerMoved(uint32_t uid, uint32_t newPos);

    //! Emitted when the layer is removed.
    //! \param uid Layer uid.
    void layerRemoved(uint32_t uid);

    //! Emitted when the layer is selected.
    //! \param uid Layer uid.
    void layerSelected(uint32_t uid);

    //! Emitted when the layer is duplicated.
    //! \param uid Layer uid.
    void layerDuplicated(uint32_t uid);

public slots:
    //! Add stroke to the selected layer of the current painting.
    //! \param stroke Stroke to add.
    void addStroke(const Stroke& stroke);

    //! Create new layer in current painting.
    //! \param name Layer name.
    //! \return Uid of the created new layer.
    uint32_t addLayer(const QString& name);

    //! Selects layer with given uid.
    //! \param uid
    void selectLayer(uint32_t uid);

    //! Move a layer with a layer uid to a new z-order position.
    //! \param uid A layer uid.
    //! \param newPos A new z-order position.
    void moveLayer(uint32_t uid, uint32_t newPos);

    //! Copies layer from a given uid to a new uid.
    //! \param fromUid A given layer uid
    //! \param toUid A new layer uid.
    void copyFromLayer(uint32_t fromUid, uint32_t toUid);

    //! Changes the current tool to a another tool.
    //! \param tool A new another tool.
    void setCurrentTool(Tool* tool);

public:
    //! Create painting.
    //! \param parent 
    explicit Painting(QObject* parent);
    ~Painting();

    //! Added the stroke to layer with the layer uid.
    //! \param uid The layer uid.
    //! \param stroke The stroke that will be added.
    void addStroke(uint32_t uid, const Stroke& stroke);

    //! Gives pen color.
    //! \return pen color.
    const QColor& getPenColor() const;

    //! Sets pen color.
    //! \param color New pen color
    void setPenColor(const QColor& color);

    //! Paints the picture with layers of given size.
    //! \param p The painter ro paint with.
    //! \param size The picture size.
    //! \param translation Translation to apply during painting.
    void drawPicture(QPainter& p, const QSize& size, const QPointF& translation) const;

    //! Returns count of strokes.
    //! \return count of strokes.
    int strokesCount() const;

    //! Rename layer with the layer uid.
    //! \param uid The layer uid.
    //! \param name The new layer name.
    void renameLayer(uint32_t uid, const QString& name);

    //! Remove layer with the layer uid.
    //! \param uid The layer uid.
    void removeLayer(uint32_t uid);

    //! Returns point to the current layer.
    //! \return Point to the current layer.
    const Layer* getCurrentLayer() const;

    //! Returns the current layer uid.
    //! \return The current layer uid.
    uint32_t getCurrentLayerId() const;

    //! Returns count of layers.
    //! \return count of layers.
    int layersCount() const;

    //! Returns the layer index in z-order with layer uid.
    //! \param uid The layer uid.
    //! \return The layer index in z-order.
    int layerIndex(uint32_t uid) const;

    //! Checks if layers.
    bool hasLayers() const;
};
