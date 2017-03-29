#include <QTime>
#include "ServerMainWindow.h"
#include "transport.h"
#include "enums.h"
#include "messages.h"

void ServerMainWindow::acceptConnection() {
    printf("NEW CONNECTION ACCEPTED!!! \n");
    QTcpSocket* clientSocket = srv.nextPendingConnection();
    static int i = 1;
    QString name = QString("player %1").arg(i++);
    clients[clientSocket] = new ClientInfo(name);
    connect(clientSocket, SIGNAL(readyRead()),this, SLOT(readyToRead()));
    connect(clientSocket, SIGNAL(disconnected()),this, SLOT(clientDisconnected()));
    clientSocket->write(SetClientNameMessage(name).encodeMessage());
    clientSocket->flush();
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
    reader.processBytes(data, {
            {STRING_MESSAGE, [this, available, socket](const QByteArray& message) {
                StringMessage m(message);
                QString dataStr = m.str;
                printf("Got data: %i bytes\n%s\n", available, message.data());
                QString s = "[" + QTime::currentTime().toString() + "] <" + clients[socket]->name + "> " + dataStr;
                textEdit->append(s);

                QByteArray answer = StringMessage(s).encodeMessage();
                for (QTcpSocket* clientSocket : clients.keys()) {
                    clientSocket->write(answer);
                    clientSocket->flush();
                }
            }},
            {PATH_MESSAGE, [this, socket](const QByteArray& message) {
                QByteArray answer = createM(PATH_MESSAGE, message);
                for (QTcpSocket* clientSocket : clients.keys()) {
                    if(clientSocket == socket){
                        continue;
                    }
                    clientSocket->write(answer);
                    clientSocket->flush();
                }
            }},
    });

}

void ServerMainWindow::clientDisconnected() {
    QTcpSocket* socket = (QTcpSocket *) sender();
    delete clients.take(socket);
    socket->deleteLater();
}
