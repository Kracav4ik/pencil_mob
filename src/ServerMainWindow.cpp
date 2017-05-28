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

HandlePair::CallbackType ServerMainWindow::multicastFunc(uint32_t messageType, QTcpSocket* ignoreSocket) {
    return [this, messageType, ignoreSocket](const QByteArray& message) {
        QByteArray answer = createM(messageType, message);
        for (QTcpSocket* clientSocket : clients.keys()) {
            if(clientSocket == ignoreSocket){
                continue;
            }
            clientSocket->write(answer);
            clientSocket->flush();
        }
    };
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
            {PATH_MESSAGE, multicastFunc(PATH_MESSAGE, socket)},
            {RENAME_LAYER_MESSAGE, multicastFunc(RENAME_LAYER_MESSAGE, socket)},
            {ADD_NEW_LAYER_MESSAGE, multicastFunc(ADD_NEW_LAYER_MESSAGE, socket)},
            {MOVE_LAYER_MESSAGE, multicastFunc(MOVE_LAYER_MESSAGE, socket)},
            {REMOVE_LAYER_MESSAGE, multicastFunc(REMOVE_LAYER_MESSAGE, socket)},
    });

}

void ServerMainWindow::clientDisconnected() {
    QTcpSocket* socket = (QTcpSocket *) sender();
    delete clients.take(socket);
    socket->deleteLater();
}
