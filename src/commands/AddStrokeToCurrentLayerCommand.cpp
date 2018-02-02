#include "AddStrokeToCurrentLayerCommand.h"
#include "Painting.h"
#include "messages.h"

AddStrokeToCurrentLayerCommand::AddStrokeToCurrentLayerCommand(Painting& painting, MessageSender& sender, const Stroke& stroke)
    : ClientCommand(painting, sender, "Add stroke"), stroke(stroke) {}

void AddStrokeToCurrentLayerCommand::undo() {
    painting.removeLastStroke(uid);
    sender.sendMessage<RemoveLastStrokeMessage>(uid.layer);
}

void AddStrokeToCurrentLayerCommand::redo() {
    uid = painting.getCurrentLayerId();
    painting.addStroke(uid, stroke);
    sendMessage<PathMessage>(stroke.color, uid.layer, stroke.isEraser, stroke.polygon);
}
