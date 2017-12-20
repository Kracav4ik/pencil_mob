#include "LayerCommandBase.h"
#include "ClientMainWindow.h"

LayerCommandBase::LayerCommandBase(Painting& painting, const QString& text, const QString& name)
    : ClientCommand(painting, text), layer(name) {}
