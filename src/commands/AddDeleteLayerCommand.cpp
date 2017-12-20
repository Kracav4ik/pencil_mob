#include "AddDeleteLayerCommand.h"
#include "ClientMainWindow.h"

AddDeleteLayerCommand::AddDeleteLayerCommand(Painting& painting, const QString& text, const QString& name, LayerId layerId)
    : LayerCommandBase(painting, text, name), layerId(layerId) {}

void AddDeleteLayerCommand::createLayer() {
    painting.addLayer(layerId, layer.getName());
    painting.getByUid(layerId)->copyFrom(layer);
}

void AddDeleteLayerCommand::deleteLayer() {
    layer.copyFrom(*painting.getByUid(layerId));
    painting.removeLayer(layerId);
}

LayerId AddDeleteLayerCommand::getLayerId() {
    return layerId;
}
