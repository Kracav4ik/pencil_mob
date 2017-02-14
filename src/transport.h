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
    uint32_t type;
    std::function<void(const QByteArray&)> callback;

    HandlePair(uint32_t type, const std::function<void(const QByteArray&)>& callback);
};

class MessageHandler {
private:
    std::vector<HandlePair> handlePairs;

public:
    MessageHandler(std::initializer_list<HandlePair> handlePairs);

    void handle(uint32_t type, const QByteArray& message) const;
};

QByteArray createM(uint32_t type, QString string);

void takeM(QByteArray message, const MessageHandler& handler);