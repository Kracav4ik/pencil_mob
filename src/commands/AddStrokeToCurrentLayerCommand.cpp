#include "AddStrokeToCurrentLayerCommand.h"
#include "Painting.h"

AddStrokeToCurrentLayerCommand::AddStrokeToCurrentLayerCommand(Painting& painting, MessageSender& sender, const Stroke& stroke)
    : ClientCommand(painting, sender, "Add stroke"), stroke(stroke) {}

void AddStrokeToCurrentLayerCommand::undo() {
    painting.removeLastStroke(uid);
}

void AddStrokeToCurrentLayerCommand::redo() {
    uid = painting.getCurrentLayerId();
    painting.addStroke(uid, stroke);
}
