#include "MoveLayerCommand.h"
#include "messages.h"

MoveLayerCommand::MoveLayerCommand(Painting& painting, ClientMainWindow& main, uint32_t uid, uint32_t oldPos, uint32_t newPos)
    : AddDeleteLayerCommand(painting, main, "Move layer", painting.getLayerNameFromUid({painting.getOurUserId(), uid}), uid), oldPos(oldPos), newPos(newPos) {}

void MoveLayerCommand::undo() {
    painting.moveLayer({painting.getOurUserId(), layerId}, oldPos);
    sendMessage<MoveLayerMessage>(layerId, oldPos);
}

void MoveLayerCommand::redo() {
    painting.moveLayer({painting.getOurUserId(), layerId}, newPos);
    sendMessage<MoveLayerMessage>(layerId, newPos);
}
