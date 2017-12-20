#pragma once

#include "LayerId.h"
#include "AddDeleteLayerCommand.h"

//! The command for duplicating layer.
class DuplicateLayerCommand : public AddDeleteLayerCommand {
private:
    LayerId from;

public:
    //! Create the command for layer.
    DuplicateLayerCommand(Painting& painting, LayerId to, const LayerId& from);

    void undo() override;

    void redo() override;
};
