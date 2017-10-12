#include "transport.h"

#include "MessageHandler.h"

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

QByteArray createMessage(QByteArray prefix, uint32_t type, const QByteArray& data) {
    prefix.append(encode(type));
    prefix.append(data);
    prefix.prepend(encode((uint32_t) prefix.length()));
//    print_debug(prefix, "<SND>");
    return prefix;
}

QByteArray createAnonymousMessage(uint32_t type, const QByteArray& data) {
    return createMessage({}, type, data);
}

QByteArray createUserMessage(uint32_t user, uint32_t type, const QByteArray& data) {
    return createMessage(encode(user), type, data);
}


void print_debug(const QByteArray& array, const char* prefix) {
    printf("%s", prefix);
    for (int i = 0; i < array.size(); ++i) {
        printf(" %02x", (uint8_t)array[i]);
    }
    printf("\n");
}

void MessageReader::processBytes(const QByteArray& bytes, const MessageHandlerFunc& handler) {
    unread += bytes;
    while (true) {
        Decoder sizeDecoder(unread);
        if (!sizeDecoder.decoded) {
            break;
        }
        uint32_t size = sizeDecoder.number;

        if (unread.size() < sizeDecoder.count + size) {
            break;
        }

        QByteArray messageBody = unread.mid(sizeDecoder.count, size);
        handler(messageBody);
        unread = unread.mid(sizeDecoder.count + size);
    }
}

void processAnonymousMessage(const QByteArray& message, const ServerMessageReader::AnonymousMessageHandlerFunc& handler) {
    Decoder typeDecoder(message);
    if (!typeDecoder.decoded) {
        return;
    }
    uint32_t type = typeDecoder.number;

    handler(type, message.mid(typeDecoder.count));
}

void ServerMessageReader::processBytes(const QByteArray& bytes, const AnonymousMessageHandlerFunc& handler) {
    MessageReader::processBytes(bytes, [&](const QByteArray& message){
        processAnonymousMessage(message, handler);
    });
}

void ClientMessageReader::processBytes(const QByteArray& bytes, MessageHandler& handler) {
    MessageReader::processBytes(bytes, [&](const QByteArray& message){
        Decoder userDecoder(message);
        if (!userDecoder.decoded) {
            return;
        }
        uint32_t user = userDecoder.number;

        processAnonymousMessage(message.mid(userDecoder.count), [&](uint32_t type, const QByteArray& msg){
            handler.handle(user, type, msg);
        });
    });
}
