#include "ClientCommand.h"

ClientCommand::ClientCommand(Painting& painting, const QString& text)
        : QUndoCommand(text), painting(painting) {}
