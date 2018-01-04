#include "RemoveLayerCommand.h"

RemoveLayerCommand::RemoveLayerCommand(Painting& painting, ClientMainWindow& main, const QString& name, uint32_t id)
    : AddDeleteLayerCommand(painting, main, "Delete layer", name, id) {}

void RemoveLayerCommand::undo() {
    createLayer();
    painting.selectLayer(layerId);
}

void RemoveLayerCommand::redo() {
    deleteLayer();
}

RemoveLayerCommand& RemoveLayerCommand::create(Painting& painting, ClientMainWindow& main, uint32_t layerId) {
    return *new RemoveLayerCommand(painting, main, painting.getLayerNameFromUid({painting.getOurUserId(), layerId}), layerId);
}
