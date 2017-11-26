#include "MessageHandler.h"

#include "enums.h"
#include "messages.h"

void MessageHandler::handle(uint32_t user, uint32_t messageType, const QByteArray& message) {
    switch (messageType) {
        case STRING_MESSAGE: {
            StringMessage msg(message);
            handleStringMessage(user, msg);
            break;
        }
        case SET_CLIENT_INFO_MESSAGE: {
            SetClientInfoMessage msg(message);
            handleSetClientInfoMessage(user, msg);
            break;
        }
        case PATH_MESSAGE: {
            PathMessage msg(message);
            handlePathMessage(user, msg);
            break;
        }
        case ADD_NEW_LAYER_MESSAGE: {
            AddNewLayerMessage msg(message);
            handleAddNewLayerMessage(user, msg);
            break;
        }
        case RENAME_LAYER_MESSAGE: {
            RenameLayerMessage msg(message);
            handleRenameLayerMessage(user, msg);
            break;
        }
        case MOVE_LAYER_MESSAGE: {
            MoveLayerMessage msg(message);
            handleMoveLayerMessage(user, msg);
            break;
        }
        case REMOVE_LAYER_MESSAGE: {
            RemoveLayerMessage msg(message);
            handleRemoveLayerMessage(user, msg);
            break;
        }
        case COPY_LAYER_MESSAGE: {
            CopyLayerMessage msg(message);
            handleCopyLayerMessage(user, msg);
            break;
        }
        case LAYER_CONTENTS_MESSAGE: {
            LayerContentsMessage msg(message);
            handleLayerContentsMessage(user, msg);
            break;
        }
    }
}
