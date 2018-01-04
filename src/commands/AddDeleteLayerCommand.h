#pragma once

#include "LayerId.h"
#include "commands/LayerCommandBase.h"

//! The base command for adding and deleting Layers.
class AddDeleteLayerCommand : public LayerCommandBase {
protected:
    uint32_t layerId;
    //! Create the command for layer.
    AddDeleteLayerCommand(Painting& painting, MessageSender& sender, const QString& text, const QString& name, uint32_t layerId);
    //! A protected method for creating layer from the params given in constructor when the command was created.
    void createLayer();
    //! A protected method for deleting layer with the name given in constructor when the command was created.
    void deleteLayer();

public:
    LayerId getLayerId();
};
