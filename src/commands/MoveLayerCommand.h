#pragma once

#include "AddDeleteLayerCommand.h"

//! The command for moving layer.
class MoveLayerCommand : public AddDeleteLayerCommand {
private:
    uint32_t newPos;

public:
    //! Create the command for layer.
    MoveLayerCommand(Painting& painting, const LayerId& oldPos, uint32_t newPos);

    void undo() override;

    void redo() override;
};
