#include "RenameLayerCommand.h"
#include "Painting.h"

RenameLayerCommand::RenameLayerCommand(Painting& painting, const QString& oldName, LayerId uid, const QString& newName)
    : AddDeleteLayerCommand(painting, "Rename layer", oldName, uid), newName(newName){}

void RenameLayerCommand::undo() {
    painting.renameLayer(layerId, layer.getName());
}

void RenameLayerCommand::redo() {
    painting.renameLayer(layerId, newName);
}
