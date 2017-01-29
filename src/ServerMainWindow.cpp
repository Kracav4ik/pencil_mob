#include <QTime>
#include "ServerMainWindow.h"

void ServerMainWindow::acceptConnection() {
    printf("NEW CONNECTION ACCEPTED!!! \n");
    client = srv.nextPendingConnection();
    connect(client, SIGNAL(readyRead()),this, SLOT(readyToRead()));
}

ServerMainWindow::ServerMainWindow():client(nullptr) {
    setupUi(this);
    connect(&srv, SIGNAL(newConnection()),this, SLOT(acceptConnection()));
    srv.listen(QHostAddress::Any, 9000);
    show();
}

void ServerMainWindow::readyToRead() {
    int available = (int) client->bytesAvailable();
    QByteArray data = client->readAll();
    printf("Got data: %i bytes\n%s\n", available, data.toStdString().c_str());
    QByteArray message = "[" + QTime::currentTime().toString().toUtf8() + "] " + data;
    textEdit->append(message);
    client->write(message);
    client->flush();
}
