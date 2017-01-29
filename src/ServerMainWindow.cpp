#include <QTime>
#include "ServerMainWindow.h"

void ServerMainWindow::acceptConnection() {
    printf("NEW CONNECTION ACCEPTED!!! \n");
    QTcpSocket* client = srv.nextPendingConnection();
    static int i = 1;
    QString name = QString("player %1").arg(i++);
    clients[client] = new ClientInfo(name);
    connect(client, SIGNAL(readyRead()),this, SLOT(readyToRead()));
    connect(client, SIGNAL(disconnected()),this, SLOT(clientDisconnected()));
}

ServerMainWindow::ServerMainWindow() {
    setupUi(this);
    connect(&srv, SIGNAL(newConnection()),this, SLOT(acceptConnection()));
    srv.listen(QHostAddress::Any, 9000);
    show();
}

void ServerMainWindow::readyToRead() {
    QTcpSocket* socket = (QTcpSocket *) sender();
    int available = (int) socket->bytesAvailable();
    QByteArray data = socket->readAll();
    printf("Got data: %i bytes\n%s\n", available, data.toStdString().c_str());
    QByteArray message = "[" + QTime::currentTime().toString().toUtf8() + "] <" + clients[socket]->name.toUtf8() + "> " + data;
    textEdit->append(message);

    for (QTcpSocket* clientSocket : clients.keys()) {
        clientSocket->write(message);
        clientSocket->flush();
    }

}

void ServerMainWindow::clientDisconnected() {
    QTcpSocket* socket = (QTcpSocket *) sender();
    delete clients.take(socket);
    socket->deleteLater();
}
