#include "AddLayerCommand.h"

void AddLayerCommand::undo() {
    deleteLayer();
}

void AddLayerCommand::redo() {
    createLayer();
}

AddLayerCommand::AddLayerCommand(Painting& painting, const QString& name, LayerId layerId)
    : AddDeleteLayerCommand(painting, "Add layer", name, layerId) {}
