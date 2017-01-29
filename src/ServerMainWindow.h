#pragma once

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

#include "ui_pencil_mob_server.h"


class ServerMainWindow : public QMainWindow, private Ui::ServerMainWindow {
Q_OBJECT
private:
    QTcpServer srv;
    QTcpSocket* client;

public:
    ServerMainWindow();

public slots:
    void readyToRead();
    void acceptConnection();
};

