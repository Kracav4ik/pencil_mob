#pragma once

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

#include "ui_pencil_mob_server.h"
#include "transport.h"

struct ClientInfo{
    QString name;

    ClientInfo(const QString &name) : name(name) {}
};

class ServerMainWindow : public QMainWindow, private Ui::ServerMainWindow {
Q_OBJECT
private:
    QTcpServer srv;
    QMap<QTcpSocket*, ClientInfo*> clients;
    MessageReader reader;

    HandlePair::CallbackType multicastFunc(uint32_t messageType, QTcpSocket* ignoreSocket=nullptr);

public:
    ServerMainWindow();

public slots:
    void readyToRead();
    void acceptConnection();
    void clientDisconnected();
};

