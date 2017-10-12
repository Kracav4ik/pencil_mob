#pragma once

#include <cstdint>

class QByteArray;

class StringMessage;
class SetClientNameMessage;
class PathMessage;
class AddNewLayerMessage;
class RenameLayerMessage;
class MoveLayerMessage;
class RemoveLayerMessage;
class CopyLayerMessage;

class MessageHandler {
public:
    void handle(uint32_t user, uint32_t messageType, const QByteArray& message);

protected:
    virtual void handleStringMessage(uint32_t user, const StringMessage& m) = 0;
    virtual void handleSetClientNameMessage(uint32_t user, const SetClientNameMessage& m) = 0;
    virtual void handlePathMessage(uint32_t user, const PathMessage& m) = 0;
    virtual void handleAddNewLayerMessage(uint32_t user, const AddNewLayerMessage& m) = 0;
    virtual void handleRenameLayerMessage(uint32_t user, const RenameLayerMessage& m) = 0;
    virtual void handleMoveLayerMessage(uint32_t user, const MoveLayerMessage& m) = 0;
    virtual void handleRemoveLayerMessage(uint32_t user, const RemoveLayerMessage& m) = 0;
    virtual void handleCopyLayerMessage(uint32_t user, const CopyLayerMessage& m) = 0;

    virtual ~MessageHandler() = default;
};
