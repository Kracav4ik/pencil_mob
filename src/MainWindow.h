#pragma once

#include <QMainWindow>
#include <QTcpServer>

#include "ui_pencil_mob.h"


class MainWindow : public QMainWindow, private Ui::MainWindow {
Q_OBJECT
private:
    QTcpServer srv;

public:
    MainWindow() {
        setupUi(this);
        connect(&srv, SIGNAL(newConnection()),this, SLOT(acceptConnection()));
        srv.listen(QHostAddress::Any, 9000);
        show();
    }

public slots:
    void acceptConnection();
    void on_pushButton_clicked();

};

