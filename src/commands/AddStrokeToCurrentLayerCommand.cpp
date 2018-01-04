#include "AddStrokeToCurrentLayerCommand.h"

AddStrokeToCurrentLayerCommand::AddStrokeToCurrentLayerCommand(Painting& painting, ClientMainWindow& main, const Stroke& stroke)
    : ClientCommand(painting, main, "Add stroke"), stroke(stroke) {}

void AddStrokeToCurrentLayerCommand::undo() {
    painting.removeLastStroke(uid);
}

void AddStrokeToCurrentLayerCommand::redo() {
    uid = painting.getCurrentLayerId();
    painting.addStroke(uid, stroke);
}
