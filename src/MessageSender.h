#pragma once

class QByteArray;

class MessageSender {
protected:
    ~MessageSender() = default;

public:
    //! Something kind of magic.
    template<typename MsgClass, typename... ArgTypes>
    void sendMessage(ArgTypes... args) {
        if(!isConnected()){
            return;
        }

        writeMessage(MsgClass(args...).encodeMessage());
    }
    virtual bool isConnected() const = 0;
    virtual void writeMessage(const QByteArray& data) = 0;
};
