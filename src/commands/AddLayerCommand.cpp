#include "AddLayerCommand.h"

void AddLayerCommand::undo() {
    deleteLayer();
}

void AddLayerCommand::redo() {
    createLayer();
}

AddLayerCommand::AddLayerCommand(Painting& painting, MessageSender& sender, const QString& name, uint32_t layerId)
    : AddDeleteLayerCommand(painting, sender, "Add layer", name, layerId) {}
