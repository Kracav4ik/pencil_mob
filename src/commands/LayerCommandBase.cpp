#include "LayerCommandBase.h"

LayerCommandBase::LayerCommandBase(Painting& painting, MessageSender& sender, const QString& text, const QString& name)
    : ClientCommand(painting, sender, text), layer(name) {}
