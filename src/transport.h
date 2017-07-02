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
    //! Number decoder.
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

//! Pair of a message type and a callback to handle messages of this type.
struct HandlePair {
    //! std::function wrapper over the callback
    typedef std::function<void(const QByteArray&)> CallbackType;

    //! The type of the message.
    uint32_t type;
    //! The callback to handle messages of this type.
    CallbackType callback;

    //!  Create handlePair.
    //! \param type The type of the message.
    //! \param callback The callback to handle messages of this type.
    HandlePair(uint32_t type, const CallbackType& callback);
};

//! Class to handle messages of different type.
class MessageHandler {
private:
    //! A list of message type - callback pairs.
    std::vector<HandlePair> handlePairs;

public:
    //! Create MessageHandler from std::initializer_list.
    //! \param handlePairs A list of message type - callback pairs.
    MessageHandler(std::initializer_list<HandlePair> handlePairs);

    //! Handle unwrapped message (without leading length and type bytes).
    //! \param type Type of the message.
    //! \param message Message bytes.
    void handle(uint32_t type, const QByteArray& message) const;
};

//! Wraps message bytes by prepending it with 7-bit encoded type and then 7-bit encoded total length.
//! \param type Type of the message.
//! \param data Message bytes.
//! \return Wrapped message.
QByteArray createM(uint32_t type, QByteArray data);

//! One-line debug print of a QByteArray with format "[prefix] [hexadecimal bytes]".
//! \param array The array to print.
//! @param prefix The prefix at the beginning.
void print_debug(const QByteArray& array, const char* prefix);

//! Reader of a message.
class MessageReader {
private:
    //! The message bytes.
    QByteArray unread;

public:
    //! Handle the message with the handler.
    //! \param message Message that be handle.
    //! \param handler Handler for message.
    void processBytes(const QByteArray& message, const MessageHandler& handler);
};

//! Abstract base class for messages.
struct MessageBase{
    //! Message type.
    const uint32_t type;

    //! Create MessageBase.
    //! \param type Message type.
    explicit MessageBase(uint32_t type) : type(type) {}

    //! Encode message.
    virtual QByteArray encodeMessage() const = 0;

    //! Virtual destructor for correct and deletion.
    virtual ~MessageBase() = default;
};
