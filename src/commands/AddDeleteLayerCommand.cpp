#include "AddDeleteLayerCommand.h"
#include "messages.h"

AddDeleteLayerCommand::AddDeleteLayerCommand(Painting& painting, ClientMainWindow& main, const QString& text, const QString& name, uint32_t layerId)
    : LayerCommandBase(painting, main, text, name), layerId(layerId) {}

void AddDeleteLayerCommand::createLayer() {
    LayerId uid{painting.getOurUserId(), layerId};
    painting.addLayer(uid, layer.getName());
    sendMessage<AddNewLayerMessage>(layerId, layer.getName());

    painting.getByUid(uid)->copyFrom(layer);
    sendMessage<LayerContentsMessage>(painting.uidFromLayer(&layer).layer, layer.getSrokes(), layer.getName());
}

void AddDeleteLayerCommand::deleteLayer() {
    LayerId uid{painting.getOurUserId(), layerId};
    layer.copyFrom(*painting.getByUid(uid));
    painting.removeLayer(uid);
    sendMessage<RemoveLayerMessage>(layerId);
}

LayerId AddDeleteLayerCommand::getLayerId() {
    return {painting.getOurUserId(), layerId};
}
