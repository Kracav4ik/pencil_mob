#include "ClientCommand.h"

ClientCommand::ClientCommand(Painting& painting, ClientMainWindow& main, const QString& text)
        : QUndoCommand(text), painting(painting), main(main) {}
