#pragma once

#include "AddDeleteLayerCommand.h"

//! The command for moving layer.
class MoveLayerCommand : public AddDeleteLayerCommand {
private:
    uint32_t newPos;
    uint32_t oldPos;

public:
    //! Create the command for layer.
    MoveLayerCommand(Painting& painting, ClientMainWindow& main, uint32_t uid, uint32_t oldPos, uint32_t newPos);

    void undo() override;

    void redo() override;
};
