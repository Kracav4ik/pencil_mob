#pragma once

#include <QByteArray>
#include <QString>
#include <functional>

QByteArray encode(uint32_t value);
struct Decoder{
    uint32_t number;
    int count;

    Decoder(QByteArray bytes);
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

void takeM(QByteArray message, const MessageHandler& handler);

void print_debug(const QByteArray& array, const char* prefix);