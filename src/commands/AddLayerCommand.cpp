#include "AddLayerCommand.h"

void AddLayerCommand::undo() {
    deleteLayer();
}

void AddLayerCommand::redo() {
    createLayer();
}

AddLayerCommand::AddLayerCommand(Painting& painting, ClientMainWindow& main, const QString& name, uint32_t layerId)
    : AddDeleteLayerCommand(painting, main, "Add layer", name, layerId) {}
