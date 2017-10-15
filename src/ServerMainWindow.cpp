#include <QTime>
#include "ServerMainWindow.h"
#include "enums.h"
#include "messages.h"
#include "Layer.h"

void ServerMainWindow::acceptConnection() {
    printf("NEW CONNECTION ACCEPTED!!! \n");
    QTcpSocket* clientSocket = srv.nextPendingConnection();
    static uint32_t userCounter = 1;
    uint32_t user = userCounter++;
    QString name = QString("player %1").arg(user);
    clients[clientSocket] = new ClientInfo(name, user);
    connect(clientSocket, SIGNAL(readyRead()),this, SLOT(readyToRead()));
    connect(clientSocket, SIGNAL(disconnected()),this, SLOT(clientDisconnected()));
    clientSocket->write(SetClientNameMessage(name).encodeMessage(user));
    clientSocket->flush();

    if (painting.hasLayers()) {
        for (const Layer* layer : painting.getLayers()) {
            if (layer->getUser() == clients[clientSocket]->user) {
                continue;
            }
            QByteArray answer = createUserMessage(layer->getUser(), ADD_NEW_LAYER_MESSAGE, AddNewLayerMessage(layer->getName()).encodeMessage());
            clientSocket->write(answer);
            clientSocket->flush();
        }
    }
}

ServerMainWindow::ServerMainWindow(): painting(this) {
    setupUi(this);
    connect(&srv, SIGNAL(newConnection()),this, SLOT(acceptConnection()));
    srv.listen(QHostAddress::Any, 9000);
    show();
}

void ServerMainWindow::readyToRead() {
    QTcpSocket* senderSocket = (QTcpSocket *) sender();
    int available = (int) senderSocket->bytesAvailable();
    QByteArray data = senderSocket->readAll();
    reader.processBytes(data, [this, available, senderSocket](uint32_t messageType, const QByteArray& message){
        if (messageType == STRING_MESSAGE) {
            StringMessage m(message);
            QString dataStr = m.str;
            printf("Got data: %i bytes\n%s\n", available, message.data());
            QString s = "[" + QTime::currentTime().toString() + "] <" + clients[senderSocket]->name + "> " + dataStr;
            textEdit->append(s);

            QByteArray answer = StringMessage(s).encodeMessage(clients[senderSocket]->user);
            for (QTcpSocket* clientSocket : clients.keys()) {
                clientSocket->write(answer);
                clientSocket->flush();
            }
        } else {
            if (messageType == ADD_NEW_LAYER_MESSAGE) {
                AddNewLayerMessage m(message);
                if (!painting.containsLayer(clients[senderSocket]->user, m.layerName)) {
                    painting.addLayer(clients[senderSocket]->user, m.layerName);
                }
            }

            QByteArray answer = createUserMessage(clients[senderSocket]->user, messageType, message);
            for (QTcpSocket* clientSocket : clients.keys()) {
                if(clientSocket == senderSocket){
                    continue;
                }
                clientSocket->write(answer);
                clientSocket->flush();
            }
        }
    });
}

void ServerMainWindow::clientDisconnected() {
    QTcpSocket* socket = (QTcpSocket *) sender();
    delete clients.take(socket);
    socket->deleteLater();
}
