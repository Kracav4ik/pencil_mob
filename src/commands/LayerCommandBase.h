#pragma once

#include "commands/ClientCommand.h"

#include "Layer.h"

//! The base class for all Layer commands.
class LayerCommandBase : public ClientCommand {
protected:
    Layer layer;

    //! Create the command for layer.
    LayerCommandBase(Painting& painting, ClientMainWindow& main, const QString& text, const QString& name);
};
