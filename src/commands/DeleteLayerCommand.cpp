#include "DeleteLayerCommand.h"
#include "Painting.h"

DeleteLayerCommand::DeleteLayerCommand(Painting& painting, const QString& name, LayerId id)
    : AddDeleteLayerCommand(painting, "Delete layer", name, id) {}

void DeleteLayerCommand::undo() {
    createLayer();
    painting.selectLayer(layerId.layer);
}

void DeleteLayerCommand::redo() {
    deleteLayer();
}

DeleteLayerCommand& DeleteLayerCommand::create(Painting& painting, LayerId layerId) {
    return *new DeleteLayerCommand(painting, painting.getLayerNameFromUid(layerId), layerId);
}
