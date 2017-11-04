#pragma once
#include <cstdint>

enum MessageType {
    STRING_MESSAGE = 1,
    SET_CLIENT_NAME_MESSAGE,
    PATH_MESSAGE,
    ADD_NEW_LAYER_MESSAGE,
    RENAME_LAYER_MESSAGE,
    MOVE_LAYER_MESSAGE,
    REMOVE_LAYER_MESSAGE,
    COPY_LAYER_MESSAGE,
};

const char* getMessageTypeString(uint32_t type);
