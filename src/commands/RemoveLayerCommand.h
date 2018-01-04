#pragma once

#include "commands/AddDeleteLayerCommand.h"

//! The command for deleting layer.
class RemoveLayerCommand : public AddDeleteLayerCommand {
private:
    //! Create the command. Made private so the command can be created from layer only.
    RemoveLayerCommand(Painting& painting, MessageSender& sender, const QString& name, uint32_t layerId);

public:
    //! Create the command for layer.
    static RemoveLayerCommand& create(Painting& painting, MessageSender& sender, uint32_t layerId);

    void undo() override;
    void redo() override;
};
