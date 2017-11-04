#include <QTime>
#include "ServerMainWindow.h"
#include "enums.h"
#include "messages.h"
#include "Layer.h"

QString addTime(const QString& s) {
    return QString("[%1] %2").arg(QTime::currentTime().toString(), s);
}

void ServerMainWindow::acceptConnection() {
    printf("NEW CONNECTION ACCEPTED!!! \n");
    QTcpSocket* clientSocket = srv.nextPendingConnection();
    static uint32_t userCounter = 1;
    uint32_t user = userCounter++;
    QString name = QString("player %1").arg(user);
    clients[clientSocket] = new ClientInfo(name, user);
    connect(clientSocket, SIGNAL(readyRead()),this, SLOT(readyToRead()));
    connect(clientSocket, SIGNAL(disconnected()),this, SLOT(clientDisconnected()));
    SetClientNameMessage nameMessage = SetClientNameMessage(name);
    QString debugLine = QString("send initial %1 to '%2'").arg(getMessageTypeString(nameMessage.type), name);
    debugEdit->append(addTime(debugLine));
    clientSocket->write(nameMessage.encodeMessage(user));
    clientSocket->flush();

    if (painting.hasLayers()) {
        for (const Layer* layer : painting.getLayers()) {
            if (layer->getUser() == clients[clientSocket]->user) {
                continue;
            }
            AddNewLayerMessage layerMessage = AddNewLayerMessage(layer->getName());
            QString layerLine = QString("send initial %1 to '%2'").arg(getMessageTypeString(layerMessage.type), name);
            debugEdit->append(addTime(layerLine));
            clientSocket->write(layerMessage.encodeMessage(layer->getUser()));
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
    QByteArray data = senderSocket->readAll();
    reader.processBytes(data, [this, senderSocket](uint32_t messageType, const QByteArray& message){
        QString messageTypeString = getMessageTypeString(messageType);
        const QString& senderName = clients[senderSocket]->name;
        QString debugLine = QString("%1 from client '%2'").arg(messageTypeString, senderName);
        debugEdit->append(addTime(debugLine));
        uint32_t senderUid = clients[senderSocket]->user;
        if (messageType == STRING_MESSAGE) {
            StringMessage m(message);
            QString dataStr = m.str;
            QString chatLine = QString("<%1> %2").arg(senderName, dataStr);
            chatEdit->append(addTime(chatLine));

            QByteArray answer = StringMessage(chatLine).encodeMessage(senderUid);
            for (QTcpSocket* clientSocket : clients.keys()) {
                QString bcastLine = QString("broadcasting %1 to '%2'").arg(messageTypeString, senderName);
                debugEdit->append(addTime(bcastLine));
                clientSocket->write(answer);
                clientSocket->flush();
            }
        } else {
            if (messageType == ADD_NEW_LAYER_MESSAGE) {
                AddNewLayerMessage m(message);
                if (!painting.containsLayer(senderUid, m.layerName)) {
                    painting.addLayer(senderUid, m.layerName);
                } else {
                    debugEdit->append(QString("WTF?! already contain layer '%1' from user id %2").arg(m.layerName).arg(senderUid));
                }
            }

            QByteArray answer = createUserMessage(senderUid, messageType, message);
            for (QTcpSocket* clientSocket : clients.keys()) {
                if(clientSocket == senderSocket){
                    continue;
                }
                QString bcastLine = QString("broadcasting %1 to '%2'").arg(messageTypeString, senderName);
                debugEdit->append(addTime(bcastLine));
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
