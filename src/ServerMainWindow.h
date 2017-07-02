#pragma once

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

#include "ui_pencil_mob_server.h"
#include "transport.h"

//! Information about client.
struct ClientInfo{
    //! Client name.
    QString name;

    //! Create information about client.
    ClientInfo(const QString &name) : name(name) {}
};

//! Server for sending message between clients.
class ServerMainWindow : public QMainWindow, private Ui::ServerMainWindow {
Q_OBJECT
private:
    //! Server.
    QTcpServer srv;
    //! Map from client socket to information about client.
    QMap<QTcpSocket*, ClientInfo*> clients;
    //! Reader of message between client and server.
    MessageReader reader;

    //! Send all clients except sender.
    //! \param messageType Message type that was sended.
    //! \param ignoreSocket Sender.
    //! \return Function for handle.
    HandlePair::CallbackType multicastFunc(uint32_t messageType, QTcpSocket* ignoreSocket=nullptr);

public:
    //! Start server.
    ServerMainWindow();

public slots:
    //! Connecting socket to server
    void readyToRead();
    //! Accepted connection.
    void acceptConnection();
    //! If client disconnect.
    void clientDisconnected();
};

