#pragma once

#include "commands/AddDeleteLayerCommand.h"

//! The command for adding layer.
class AddLayerCommand : public AddDeleteLayerCommand {
public:
    //! Create the command for layer.
    AddLayerCommand(Painting& painting, MessageSender& sender, const QString& name, uint32_t layerId);

    void undo() override;
    void redo() override;
};
