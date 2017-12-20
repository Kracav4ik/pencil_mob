#pragma once

#include "commands/AddDeleteLayerCommand.h"

//! The command for deleting layer.
class DeleteLayerCommand : public AddDeleteLayerCommand {
private:
    //! Create the command. Made private so the command can be created from layer only.
    DeleteLayerCommand(Painting& painting, const QString& name, LayerId layerId);

public:
    //! Create the command for layer.
    static DeleteLayerCommand& create(Painting& painting, LayerId layerId);

    void undo() override;
    void redo() override;
};
