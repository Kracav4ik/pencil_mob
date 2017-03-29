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

uint32_t decodeAndShift(QByteArray& array) {
    Decoder decoder(array);
    array = array.mid(decoder.count);
    return decoder.number;
}

QByteArray createM(uint32_t type, QByteArray data) {
    QByteArray out;
    out.append(encode(type));
    out.append(data);
    out.prepend(encode((uint32_t) out.length()));
//    print_debug(out, "<SND>");
    return out;
}

void print_debug(const QByteArray& array, const char* prefix) {
    printf("%s", prefix);
    for (int i = 0; i < array.size(); ++i) {
        printf(" %02x", (uint8_t)array[i]);
    }
    printf("\n");
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

HandlePair::HandlePair(uint32_t type, const CallbackType& callback)
        : type(type), callback(callback) {}

void MessageReader::processBytes(const QByteArray& message, const MessageHandler& handler) {
    QByteArray preMsg = unread + message;
    while (true) {
        Decoder sizeDecoder(preMsg);
        if (!sizeDecoder.decoded){
            break;
        }
        uint32_t size = sizeDecoder.number;

        QByteArray messageContents = preMsg.mid(sizeDecoder.count);
        Decoder typeDecoder(messageContents);
        if (!typeDecoder.decoded){
            break;
        }
        uint32_t type = typeDecoder.number;

        if (size > messageContents.size()) {
            break;
        }
        handler.handle(type, messageContents.left(size).mid(typeDecoder.count));
        preMsg = messageContents.mid(size);
    }
    unread = preMsg;
}
