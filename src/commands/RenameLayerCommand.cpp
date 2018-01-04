#include "RenameLayerCommand.h"
#include "Painting.h"
#include "messages.h"

RenameLayerCommand::RenameLayerCommand(
    Painting& painting, MessageSender& sender, const QString& oldName, uint32_t uid, const QString& newName)
    : AddDeleteLayerCommand(painting, sender, "Rename layer", oldName, uid), newName(newName){}

void RenameLayerCommand::undo() {
    painting.renameLayer({painting.getOurUserId(), layerId}, layer.getName());
    sendMessage<RenameLayerMessage>(painting.getCurrentLayerId().layer, layer.getName());
}

void RenameLayerCommand::redo() {
    painting.renameLayer({painting.getOurUserId(), layerId}, newName);
    sendMessage<RenameLayerMessage>(painting.getCurrentLayerId().layer, newName);
}
