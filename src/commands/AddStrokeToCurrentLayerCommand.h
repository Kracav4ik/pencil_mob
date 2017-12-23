#pragma once

#include "LayerId.h"
#include "Stroke.h"
#include "ClientCommand.h"

class AddStrokeToCurrentLayerCommand : public ClientCommand {
private:
    Stroke stroke;
    LayerId uid;
    
public:
    AddStrokeToCurrentLayerCommand(Painting& painting, const Stroke& stroke);

    void undo() override;
    void redo() override;
};
