#include "ClientMainWindow.h"
#include "transport.h"
#include "enums.h"

void ClientMainWindow::on_buttonSend_clicked(){
    if(!isConnected()){
        return;
    }
    if(lineEdit->text().isEmpty()){
        return;
    }
    client->write(createM(STRING_MESSAGE, lineEdit->text().toUtf8()));
    client->flush();
    lineEdit->setText("");
}

void ClientMainWindow::on_buttonConnect_clicked(){
    buttonConnect->setEnabled(false);
    client->connectToHost("localhost", 9000);
}

ClientMainWindow::ClientMainWindow():client(new QTcpSocket(this)) {
    client->setObjectName("socket");
    setupUi(this);
    show();
}

void ClientMainWindow::on_socket_readyRead() {
    takeM(client->readAll(), {
            {STRING_MESSAGE, [this](const QByteArray& message){
                textEdit->append(message);
            }},
            {SET_CLIENT_NAME, [this](const QByteArray& message){
                setWindowTitle("Name is " + message);
            }},
            {PATH_MESSAGE, [this](const QByteArray& message){
                QByteArray m = message;
                uint8_t r = (uint8_t) m[0];
                uint8_t g = (uint8_t) m[1];
                uint8_t b = (uint8_t) m[2];
                m = m.mid(3);

                Decoder decoder(m);
                uint32_t pointsCount = decoder.number;
                m = m.mid(decoder.count);

                QPolygon p;
                for (int i = 0; i < pointsCount; ++i) {
                    decoder = Decoder(m);
                    uint32_t x = decoder.number;
                    m = m.mid(decoder.count);

                    decoder = Decoder(m);
                    uint32_t y = decoder.number;
                    m = m.mid(decoder.count);

                    p << QPoint(x, y);
                }
                widget->addStroke(Stroke(QColor(r, g, b), p));
            }},
    });
}

void ClientMainWindow::on_socket_connected() {
    buttonSend->setEnabled(true);
    printf("Connected\n");
}

void ClientMainWindow::on_socket_error(QAbstractSocket::SocketError error) {
    printf("Errrrrrror: %d\n", error);
}

void ClientMainWindow::on_socket_stateChanged(QAbstractSocket::SocketState state) {
    printf("Staaaaaate: %d\n", state);
    switch (state){
        case QAbstractSocket::UnconnectedState:
            buttonSend->setEnabled(false);
            buttonConnect->setEnabled(true);
            break;
        default:
            break;
    }
}

void ClientMainWindow::on_widget_debugInfo(int linesCount, int paintTime) {
    debug->setText(QString("linesCount: %1, paintTime: %2 ms").arg(linesCount).arg(paintTime));
}

void ClientMainWindow::on_widget_strokeFinished(const Stroke& stroke) {
    if(!isConnected()){
        return;
    }
    const QColor& color = stroke.color;
    const QPolygon& polygon = stroke.polygon;
    QByteArray array;
    array.append((uint8_t) color.red()).append((uint8_t) color.green()).append((uint8_t) color.blue());
    array.append(encode((uint32_t) polygon.size()));
    for (const QPoint& point : polygon) {
        array.append(encode((uint32_t) point.x())).append(encode((uint32_t) point.y()));
    }
    client->write(createM(PATH_MESSAGE, array));
    client->flush();
}

bool ClientMainWindow::isConnected() {
    return buttonSend->isEnabled();
}
