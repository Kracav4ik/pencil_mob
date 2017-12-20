#include "DuplicateLayerCommand.h"
#include "Painting.h"

DuplicateLayerCommand::DuplicateLayerCommand(Painting& painting, LayerId to, const LayerId& from)
    : AddDeleteLayerCommand(painting, "Duplicate layer", painting.getLayerNameFromUid(from), to), from(from) {}

void DuplicateLayerCommand::undo() {
    deleteLayer();
}

void DuplicateLayerCommand::redo() {
    createLayer();
    painting.copyFromLayer(from, layerId);
}
