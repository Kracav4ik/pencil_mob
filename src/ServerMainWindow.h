#pragma once

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

#include "ui_pencil_mob_server.h"
#include "transport.h"
#include "Painting.h"

//! Information about client.
struct ClientInfo{
    //! Client name.
    QString name;
    uint32_t user;

    //! Create information about client.
    ClientInfo(const QString& name, uint32_t user): name(name), user(user) {}
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
    ServerMessageReader reader;

    Painting painting;

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
