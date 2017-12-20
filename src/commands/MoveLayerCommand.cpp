#include "MoveLayerCommand.h"
#include "Painting.h"

MoveLayerCommand::MoveLayerCommand(Painting& painting, const LayerId& oldPos, uint32_t newPos)
    : AddDeleteLayerCommand(painting, "Move layer", painting.getLayerNameFromUid(oldPos), oldPos), newPos(newPos) {}

void MoveLayerCommand::undo() {
    painting.moveLayer({layerId.user, newPos}, layerId.layer);
}

void MoveLayerCommand::redo() {
    painting.moveLayer(layerId, newPos);
}
