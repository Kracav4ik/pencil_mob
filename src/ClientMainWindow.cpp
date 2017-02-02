#include "ClientMainWindow.h"
#include "transport.h"

void ClientMainWindow::on_buttonSend_clicked(){
    if(lineEdit->text().isEmpty()){
        return;
    }
    client->write(createM(7777777, lineEdit->text()));
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
    textEdit->append(takeM(client->readAll()));
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
