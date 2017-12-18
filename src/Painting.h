#pragma once

#include "LayerId.h"

#include <QColor>
#include <QPicture>
#include <QVector>
#include <QHash>
#include <QDebug>

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
    QHash<LayerId, Layer*> uidToLayer;
    QHash<uint32_t, bool> userToVisible;
    //! Layers z-order.
    QVector<LayerId> zOrder;

    uint32_t ourUserId = UINT32_MAX;
    //! Layer on which we're drawing.
    LayerId currentLayer = LayerId(ourUserId, NO_LAYER);
    //! The next one layer uid if we add or duplicate the layer.
    uint32_t nextLayerUid = 1000;
    //! Tool which we're drawing.
    Tool* currentTool = nullptr;

    //! Returned layer from layer uid.
    //! \param uid layer uid
    //! \return Layer.
    const Layer* getByUid(LayerId uid) const;
    //! Returned layer from layer uid.(non-const)
    //! \param uid layer uid
    //! \return Layer.
    Layer* getByUid(LayerId uid);

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

    //! Emitted when the new layer on another client is added.
    //! \param uid New layer uid.
    //! \param name New layer name.
    void layerAdded(LayerId uid, const QString& name);

    //! Emitted when the new layer is added to our client.
    //! \param uid New layer uid.
    //! \param name New layer name.
    void ownLayerAdded(uint32_t uid, const QString& name);

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

    void userAdded(uint32_t uid);

public slots:
    //! Add stroke to the selected layer of the current painting.
    //! \param stroke Stroke to add.
    void addOwnStroke(const Stroke& stroke);

    //! Create new layer in current painting.
    //! \param name Layer name.
    //! \return Uid of the created new layer.
    LayerId addLayer(const QString& name);
    void addLayer(LayerId idx, const QString& name);

    void changingUserVisible(uint32_t user, bool visible);
    //! Selects layer with given uid.
    //! \param uid
    void selectLayer(uint32_t uid);

    //! Move a layer with a layer uid to a new z-order position.
    //! \param uid A layer uid.
    //! \param newPos A new z-order position.
    void moveLayer(LayerId uid, uint32_t newPos);

    //! Copies layer from a given uid to a new uid.
    //! \param fromUid A given layer uid
    //! \param toUid A new layer uid.
    void copyFromLayer(LayerId fromUid, LayerId toUid);

    //! Changes the current tool to a another tool.
    //! \param tool A new another tool.
    void setCurrentTool(Tool* tool);

public:
    //! Constant when we did not choose a current layer.
    static const uint32_t NO_LAYER;

    //! Create painting.
    //! \param parent
    explicit Painting(QObject* parent);
    ~Painting();

    //! Added the stroke to layer with the layer uid.
    //! \param uid The layer uid.
    //! \param stroke The stroke that will be added.
    void addStroke(LayerId uid, const Stroke& stroke);

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
    void renameLayer(LayerId uid, const QString& name);

    //! Remove layer with the layer uid.
    //! \param uid The layer uid.
    void removeLayer(LayerId uid);

    //! Returns point to the current layer.
    //! \return Point to the current layer.
    const Layer* getCurrentLayer() const;

    //! Returns the current layer uid.
    //! \return The current layer uid.
    LayerId getCurrentLayerId() const;

    QList<Layer*> getLayers() const ;

    uint32_t getOurUserId() const;
    void setOurUserId(uint32_t newUserId);

    //! Returns count of layers.
    //! \return count of layers.
    int layersCount() const;

    //! Returns the layer index in z-order with layer uid.
    //! \param uid The layer uid.
    //! \return The layer index in z-order.
    int layerIndex(LayerId uid) const;

    //! Checks if Painting has our layers.
    bool hasOwnLayers() const;

    bool containsUser(uint32_t user);

    bool containsLayer(LayerId uid);

    LayerId uidFromLayer(const Layer* layer) const;

    uint32_t getTopOwnLayer(uint32_t ignoreLayer=NO_LAYER);
};
