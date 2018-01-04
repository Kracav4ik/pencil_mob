#include "DuplicateLayerCommand.h"
#include "messages.h"
#include "Painting.h"

DuplicateLayerCommand::DuplicateLayerCommand(Painting& painting, MessageSender& sender, uint32_t to, LayerId from)
    : AddDeleteLayerCommand(painting, sender, "Duplicate layer", painting.getLayerNameFromUid(from), to), from(from) {}

void DuplicateLayerCommand::undo() {
    deleteLayer();
}

void DuplicateLayerCommand::redo() {
    createLayer();
    painting.copyFromLayer(from, {painting.getOurUserId(), layerId});
    sendMessage<CopyLayerMessage>(from.user, from.layer, layerId);
}
