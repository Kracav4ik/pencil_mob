#pragma once

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

#include "ui_pencil_mob.h"


class ClientMainWindow : public QMainWindow, private Ui::ClientMainWindow {
Q_OBJECT
private:
    QTcpServer srv;
    QTcpSocket* client;

public:
    ClientMainWindow();

public slots:
    void readyToRead();
    void acceptConnection();
    void on_pushButton_clicked();

};

