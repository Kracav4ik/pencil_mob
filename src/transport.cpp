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

QString takeM(QByteArray message) {
    Decoder decoder(message);
    uint32_t mLen = decoder.number;

    message = message.mid(decoder.count);
    decoder = Decoder(message);

    uint32_t type = decoder.number;
    printf("  type is %d\n", type);
    message = message.left(mLen).mid(decoder.count);

    return message;
}


