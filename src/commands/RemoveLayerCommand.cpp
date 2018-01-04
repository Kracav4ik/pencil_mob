#include "RemoveLayerCommand.h"
#include "Painting.h"

RemoveLayerCommand::RemoveLayerCommand(Painting& painting, MessageSender& sender, const QString& name, uint32_t id)
    : AddDeleteLayerCommand(painting, sender, "Delete layer", name, id) {}

void RemoveLayerCommand::undo() {
    createLayer();
    painting.selectLayer(layerId);
}

void RemoveLayerCommand::redo() {
    deleteLayer();
}

RemoveLayerCommand& RemoveLayerCommand::create(Painting& painting, MessageSender& sender, uint32_t layerId) {
    return *new RemoveLayerCommand(painting, sender, painting.getLayerNameFromUid({painting.getOurUserId(), layerId}), layerId);
}
