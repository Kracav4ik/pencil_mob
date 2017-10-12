#pragma once

#include <QByteArray>
#include <QString>
#include <functional>

//! Encoding a number to bytes.
//! \param value The number that will be encoded.
//! \return The encoded bytes from number.
QByteArray encode(uint32_t value);

//! A class for decoding numbers over a network.
struct Decoder{
    //! Number decoded.
    uint32_t number = 0;
    //! Count of bytes.
    int count = 0;
    //! If successful decoding, have to be true.
    bool decoded = false;

    //! Decoding a number from bytes.
    //! \tparam Array The type for decoding bytes.
    //! \param bytes The bytes that will be decoded to number.
    template <typename Array>
    explicit Decoder(const Array& bytes) {
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
//! Decodes a number and shifts.
//! \param array The bytes that will be decoded to number.
//! \return Decoded number.
uint32_t decodeAndShift(QByteArray& array);

//! Wraps message bytes by prepending it with 7-bit encoded type and then 7-bit encoded total length.
//! \param type Type of the message.
//! \param data Message bytes.
//! \return Wrapped message.
QByteArray createAnonymousMessage(uint32_t type, const QByteArray& data);
QByteArray createUserMessage(uint32_t user, uint32_t type, const QByteArray& data);

//! One-line debug print of a QByteArray with format "[prefix] [hexadecimal bytes]".
//! \param array The array to print.
//! \param prefix The prefix at the beginning.
void print_debug(const QByteArray& array, const char* prefix);

//! Reader of a message.
class MessageReader {
private:
    //! The message bytes.
    QByteArray unread;

protected:
    typedef std::function<void (const QByteArray& message)> MessageHandlerFunc;
    void processBytes(const QByteArray& bytes, const MessageHandlerFunc& handler);
};

class ServerMessageReader : public MessageReader {
public:
    typedef std::function<void (uint32_t messageType, const QByteArray& message)> AnonymousMessageHandlerFunc;

    void processBytes(const QByteArray& bytes, const AnonymousMessageHandlerFunc& handler);
};

class MessageHandler;
class ClientMessageReader : public MessageReader {
public:
    void processBytes(const QByteArray& bytes, MessageHandler& handler);
};

//! Abstract base class for messages.
struct MessageBase {
    //! Message type.
    const uint32_t type;

    //! Create MessageBase.
    //! \param type Message type.
    explicit MessageBase(uint32_t type) : type(type) {}

    QByteArray encodeMessage(uint32_t user) const {
        return createUserMessage(user, type, encodeMessageBody());
    }

    QByteArray encodeMessage() const {
        return createAnonymousMessage(type, encodeMessageBody());
    }

    //! Encode message.
    virtual QByteArray encodeMessageBody() const = 0;

    virtual ~MessageBase() = default;
};
