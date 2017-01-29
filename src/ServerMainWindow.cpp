#include <QTime>
#include "ServerMainWindow.h"
#include "transport.h"

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
    QString dataStr = takeM(data);
    printf("Got data: %i bytes\n%s\n", available, dataStr.toUtf8().data());
    QString s = "[" + QTime::currentTime().toString() + "] <" + clients[socket]->name + "> " + dataStr;
    textEdit->append(s);

    QByteArray message = createM(666, s);
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
