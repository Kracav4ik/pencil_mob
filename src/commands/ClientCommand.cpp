#include "ClientCommand.h"

ClientCommand::ClientCommand(Painting& painting, MessageSender& sender, const QString& text)
        : QUndoCommand(text), painting(painting), sender(sender) {}
