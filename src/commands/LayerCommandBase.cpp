#include "LayerCommandBase.h"

LayerCommandBase::LayerCommandBase(Painting& painting, ClientMainWindow& main, const QString& text, const QString& name)
    : ClientCommand(painting, main, text), layer(name) {}
