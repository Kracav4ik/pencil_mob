#pragma once

#include "AddDeleteLayerCommand.h"

//! The command for renaming layer.
class RenameLayerCommand : public AddDeleteLayerCommand {
private:
    QString newName;

public:
    //! Create the command for layer.
    RenameLayerCommand(Painting& painting, MessageSender& sender, const QString& oldName, uint32_t uid, const QString& newName);

    void undo() override;

    void redo() override;
};
