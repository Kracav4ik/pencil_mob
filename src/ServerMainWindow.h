#pragma once

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

#include "ui_pencil_mob_server.h"

struct ClientInfo{
    QString name;

    ClientInfo(const QString &name) : name(name) {}
};

class ServerMainWindow : public QMainWindow, private Ui::ServerMainWindow {
Q_OBJECT
private:
    QTcpServer srv;
    QMap<QTcpSocket*, ClientInfo*> clients;
public:
    ServerMainWindow();

public slots:
    void readyToRead();
    void acceptConnection();
    void clientDisconnected();
};

