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
    connect(clientSocket, &QTcpSocket::readyRead,this, &readyToRead);
    connect(clientSocket, &QTcpSocket::disconnected,this, &clientDisconnected);
    SetClientInfoMessage nameMessage = SetClientInfoMessage(name);
    QString debugLine = QString("send initial %1 to '%2'").arg(getMessageTypeString(nameMessage.type), name);
    debugEdit->append(addTime(debugLine));
    clientSocket->write(nameMessage.encodeMessage(user));
    clientSocket->flush();

    if (painting.layersCount() > 0) {
        for (const Layer* layer : painting.getLayers()) {
            LayerId layerId = painting.uidFromLayer(layer);
            if (layerId.user == clients[clientSocket]->user) {
                continue;
            }
            LayerContentsMessage layerContents(layerId.layer, layer->getSrokes(), layer->getName());
            QString layerLine = QString("send initial %1 to '%2'").arg(getMessageTypeString(layerContents.type), name);
            debugEdit->append(addTime(layerLine));
            clientSocket->write(layerContents.encodeMessage(layerId.user));
            clientSocket->flush();
        }
    }
}

ServerMainWindow::ServerMainWindow(): painting(this) {
    setupUi(this);
    connect(&srv, &QTcpServer::newConnection, this, &acceptConnection);
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
                QString bcastLine = QString("broadcasting %1 to '%2'").arg(messageTypeString, clients[clientSocket]->name);
                debugEdit->append(addTime(bcastLine));
                clientSocket->write(answer);
                clientSocket->flush();
            }
        } else {
            switch (messageType) {
                case ADD_NEW_LAYER_MESSAGE: {
                    AddNewLayerMessage m(message);
                    painting.addLayer({senderUid, m.layerId}, m.layerName);
                    break;
                }
                case MOVE_LAYER_MESSAGE: {
                    MoveLayerMessage m(message);
                    painting.moveLayer({senderUid, m.layerId}, m.newPos);
                    break;
                }
                case REMOVE_LAYER_MESSAGE: {
                    RemoveLayerMessage m(message);
                    painting.removeLayer({senderUid, m.layerId});
                    break;
                }
                case LAYER_CONTENTS_MESSAGE: {
                    LayerContentsMessage m(message);
                    LayerId layer(senderUid, m.layerId);
                    if (!painting.containsLayer({senderUid, m.layerId})) {
                        painting.addLayer(layer, m.layerName);
                        for (const Stroke& stroke : m.strokes) {
                            painting.addStroke(layer, stroke);
                        }
                    } else {
                        debugEdit->append(
                            QString("WTF?! already contain layer '%1' id %2 from user id %3")
                                .arg(m.layerName)
                                .arg(m.layerId)
                                .arg(senderUid)
                        );
                    }
                    break;
                }
                case PATH_MESSAGE: {
                    PathMessage m(message);
                    painting.addStroke({senderUid, m.layerId}, Stroke(m.color, m.isEraser, m.points));
                    break;
                }
                default: /* do nothing */;
            }

            QByteArray answer = createUserMessage(senderUid, messageType, message);
            for (QTcpSocket* clientSocket : clients.keys()) {
                if(clientSocket == senderSocket){
                    continue;
                }
                QString bcastLine = QString("broadcasting %1 to '%2'").arg(messageTypeString, clients[clientSocket]->name);
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
