#include "transport.h"

QByteArray encode(uint32_t value) {
    QByteArray array;
    bool last_byte = true;

    do {
        char cur_byte = (char) (value & 127);
        value >>= 7;
        if (last_byte) {
            last_byte = false;
        } else {
            cur_byte |= 128;
        }
        array.prepend(cur_byte);
    } while (value);

    return array;
}

Decoder::Decoder(QByteArray bytes)
        : number(0)
        , count(0) {
    char byte;

    do {
        byte = bytes[count++];
        number <<= 7;
        number |= byte & 127;
    } while (byte & 128);
}

QByteArray createM(uint32_t type, QString string) {
    QByteArray out;
    out.append(encode(type));
    out.append(string);
    out.prepend(encode((uint32_t) out.length()));
    return out;
}

void takeM(QByteArray message, const MessageHandler& handler) {
    Decoder decoder(message);
    uint32_t mLen = decoder.number;

    message = message.mid(decoder.count);
    decoder = Decoder(message);

    uint32_t type = decoder.number;
    printf("  type is %d\n", type);
    handler.handle(type, message.left(mLen).mid(decoder.count));
}


MessageHandler::MessageHandler(std::initializer_list<HandlePair> handlePairs)
        : handlePairs(handlePairs) {}

void MessageHandler::handle(uint32_t type, const QByteArray& message) const {
    for(HandlePair pair : handlePairs){
        if(type == pair.type){
            pair.callback(message);
        }
    }
}

HandlePair::HandlePair(uint32_t type, const std::function<void(const QByteArray&)>& callback)
        : type(type), callback(callback) {}
