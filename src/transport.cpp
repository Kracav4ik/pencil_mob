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

uint32_t decode(QByteArray bytes) {
    uint32_t number = 0;
    int count = 0;
    char byte;

    do {
        byte = bytes[count++];
        number <<= 7;
        number |= byte & 127;
    } while (byte & 128);

    return number;
}
