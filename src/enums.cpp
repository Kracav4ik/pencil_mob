#include "enums.h"

const char* getMessageTypeString(uint32_t type) {
    switch (type) {
        case STRING_MESSAGE: return "StringMessage";
        case SET_CLIENT_NAME_MESSAGE: return "SetClientNameMessage";
        case PATH_MESSAGE: return "PathMessage";
        case ADD_NEW_LAYER_MESSAGE: return "AddNewLayerMessage";
        case RENAME_LAYER_MESSAGE: return "RenameLayerMessage";
        case MOVE_LAYER_MESSAGE: return "MoveLayerMessage";
        case REMOVE_LAYER_MESSAGE: return "RemoveLayerMessage";
        case COPY_LAYER_MESSAGE: return "CopyLayerMessage";
    }
    return "<invalid>";
}
