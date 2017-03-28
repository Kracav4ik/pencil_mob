#pragma once

#include <QByteArray>
#include <QString>
#include <functional>

QByteArray encode(uint32_t value);
struct Decoder{
    uint32_t number = 0;
    int count = 0;
    bool decoded = false;

    template <typename Array>
    Decoder(const Array& bytes) {
        char byte;

        do {
            if (count >= bytes.size()){
                count = 0;
                number = 0;
                return;
            }
            byte = bytes[count++];
            number <<= 7;
            number |= byte & 127;
        } while (byte & 128);

        decoded = true;
    }

};

struct HandlePair {
    typedef std::function<void(const QByteArray&)> CallbackType;

    uint32_t type;
    CallbackType callback;

    HandlePair(uint32_t type, const CallbackType& callback);
};

class MessageHandler {
private:
    std::vector<HandlePair> handlePairs;

public:
    MessageHandler(std::initializer_list<HandlePair> handlePairs);

    void handle(uint32_t type, const QByteArray& message) const;
};

QByteArray createM(uint32_t type, QByteArray data);

void print_debug(const QByteArray& array, const char* prefix);

class MessageReader {
private:
    QByteArray unread;

public:
    void processBytes(const QByteArray& message, const MessageHandler& handler);
};
