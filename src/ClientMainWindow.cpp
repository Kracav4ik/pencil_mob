#include "ClientMainWindow.h"
#include "transport.h"
#include "enums.h"
#include "widgets/ColorChooserWidget.h"
#include "messages.h"

void ClientMainWindow::on_buttonSend_clicked(){
    if(!isConnected()){
        return;
    }
    if(lineEdit->text().isEmpty()){
        return;
    }
    client->write(StringMessage(lineEdit->text()).encodeMessage());
    client->flush();
    lineEdit->setText("");
}

void ClientMainWindow::on_buttonConnect_clicked(){
    buttonConnect->setEnabled(false);
    client->connectToHost("localhost", 9000);
}

ClientMainWindow::ClientMainWindow(): client(new QTcpSocket(this)), colorChooser(new ColorChooserWidget(this)) {
    client->setObjectName("socket");
    colorChooser->setObjectName("colorChooser");
    setupUi(this);
    colorChooser->selectColor(canvas->getPenColor());
    addDockWidget(Qt::RightDockWidgetArea, colorChooser);
    menuView->addAction(colorChooser->toggleViewAction());
    show();
}

void ClientMainWindow::on_socket_readyRead() {
    reader.processBytes(client->readAll(), {
            {STRING_MESSAGE, [this](const QByteArray& message){
                StringMessage m(message);
                textEdit->append(m.str);
            }},
            {SET_CLIENT_NAME, [this](const QByteArray& message){
                SetClientNameMessage m(message);
                setWindowTitle("Name is " + m.name);
            }},
            {PATH_MESSAGE, [this](const QByteArray& message){
                PathMessage m(message);
                canvas->addStroke(Stroke(QColor(m.r, m.g, m.b), m.points));
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

void ClientMainWindow::on_canvas_debugInfo(int linesCount, int paintTime) {
    debug->setText(QString("linesCount: %1, paintTime: %2 ms").arg(linesCount).arg(paintTime));
}

void ClientMainWindow::on_canvas_strokeFinished(const Stroke& stroke) {
    if(!isConnected()){
        return;
    }
    uint8_t r = (uint8_t) stroke.color.red();
    uint8_t g = (uint8_t) stroke.color.green();
    uint8_t b = (uint8_t) stroke.color.blue();
    client->write(PathMessage(r, g, b, stroke.polygon).encodeMessage());
    client->flush();
}

bool ClientMainWindow::isConnected() {
    return buttonSend->isEnabled();
}

void ClientMainWindow::on_colorChooser_colorSelected(const QColor& color) {
    canvas->setPenColor(color);
}
