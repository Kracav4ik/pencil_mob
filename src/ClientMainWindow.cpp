#include "ClientMainWindow.h"
void ClientMainWindow::on_pushButton_clicked(){
    if(lineEdit->text().isEmpty()){
        return;
    }
    textEdit->setText(textEdit->toPlainText() + "\n" + lineEdit->text());
    lineEdit->setText("");
}

void ClientMainWindow::acceptConnection() {
    printf("NEW CONNECTION ACCEPTED!!! \n");
    client = srv.nextPendingConnection();
    connect(client, SIGNAL(readyRead()),this, SLOT(readyToRead()));
}

ClientMainWindow::ClientMainWindow():client(nullptr) {
    setupUi(this);
    connect(&srv, SIGNAL(newConnection()),this, SLOT(acceptConnection()));
    srv.listen(QHostAddress::Any, 9000);
    show();
}

void ClientMainWindow::readyToRead() {
    int available = (int) client->bytesAvailable();
    printf("Got data: %i bytes\n%s\n", available, client->readAll().toStdString().c_str());
    client->write(
            "HTTP/1.1 200 OK\n"
            "Content-Type: text/html; charset=UTF-8\n"
            "Connection: close\n"
            "\n"
            "<html><body><h1>Hello, world!</h1></body></html>"
    );
    client->flush();
    client->close();
}
