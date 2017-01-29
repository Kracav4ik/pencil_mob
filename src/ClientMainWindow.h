#pragma once

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

#include "ui_pencil_mob.h"


class ClientMainWindow : public QMainWindow, private Ui::ClientMainWindow {
Q_OBJECT
private:
    QTcpSocket* client;

public:
    ClientMainWindow();

public slots:
    void on_socket_readyRead();
    void on_buttonSend_clicked();
    void on_buttonConnect_clicked();
    void on_socket_connected();
    void on_socket_error(QAbstractSocket::SocketError error);
    void on_socket_stateChanged(QAbstractSocket::SocketState state);

};

