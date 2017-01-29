#pragma once

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

#include "ui_pencil_mob.h"


class MainWindow : public QMainWindow, private Ui::MainWindow {
Q_OBJECT
private:
    QTcpServer srv;
    QTcpSocket* client;

public:
    MainWindow();

public slots:
    void readyToRead();
    void acceptConnection();
    void on_pushButton_clicked();

};

