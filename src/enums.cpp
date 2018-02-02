#include "enums.h"

const char* getMessageTypeString(uint32_t type) {
    switch (type) {
        case STRING_MESSAGE: return "StringMessage";
        case SET_CLIENT_INFO_MESSAGE: return "SetClientInfoMessage";
        case PATH_MESSAGE: return "PathMessage";
        case ADD_NEW_LAYER_MESSAGE: return "AddNewLayerMessage";
        case RENAME_LAYER_MESSAGE: return "RenameLayerMessage";
        case MOVE_LAYER_MESSAGE: return "MoveLayerMessage";
        case REMOVE_LAYER_MESSAGE: return "RemoveLayerMessage";
        case REMOVE_LAST_STROKE_MESSAGE: return "RemoveLastStrokeMessage";
        case COPY_LAYER_MESSAGE: return "CopyLayerMessage";
        case LAYER_CONTENTS_MESSAGE: return "LayerContentsMessage";
    }
    return "<invalid>";
}
