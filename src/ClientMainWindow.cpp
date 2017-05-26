#include "ClientMainWindow.h"
#include "transport.h"
#include "enums.h"
#include "widgets/ColorChooserWidget.h"
#include "widgets/ToolSelectorWidget.h"
#include "widgets/LayersWidget.h"
#include "messages.h"
#include "TextProgress.h"
#include "Layer.h"
#include <QInputDialog>

ClientMainWindow::ClientMainWindow()
        : client(new QTcpSocket(this))
        , colorChooser(new ColorChooserWidget(this))
        , toolSelector(new ToolSelectorWidget(this))
        , layersWidget(new LayersWidget(this))
        , painting(this)
{
    client->setObjectName("socket");
    colorChooser->setObjectName("colorChooser");
    toolSelector->setObjectName("toolSelector");
    layersWidget->setObjectName("layersWidget");
    painting.setObjectName("painting");
    setupUi(this);

    addDockWidget(Qt::RightDockWidgetArea, colorChooser);
    addDockWidget(Qt::LeftDockWidgetArea, toolSelector);
    addDockWidget(Qt::LeftDockWidgetArea, layersWidget);
    menuView->addAction(colorChooser->toggleViewAction());
    menuView->addAction(toolSelector->toggleViewAction());
    menuView->addAction(layersWidget->toggleViewAction());
    canvas->setPainting(&painting);

    // connect() hell
    connect(&painting, SIGNAL(changed()), canvas, SLOT(update()));
    connect(&painting, SIGNAL(penColorChanged(const QColor&)), &toolSelector->penTool, SLOT(setPenColor(const QColor&)));
    for (Tool* tool : toolSelector->allTools()) {
        connect(tool, SIGNAL(strokeFinished(const Stroke&)), &painting, SLOT(addStroke(const Stroke&)));
        connect(tool, SIGNAL(strokeFinished(const Stroke&)), this, SLOT(strokeFinished(const Stroke&)));
        connect(tool, SIGNAL(needRepaint()), canvas, SLOT(update()));
    }
    connect(toolSelector, SIGNAL(toolSelected(Tool*)), &painting, SLOT(setCurrentTool(Tool*)));
    connect(canvas, SIGNAL(beginDrag()), toolSelector, SLOT(beginDrag()));
    connect(canvas, SIGNAL(drag(const QPoint&)), toolSelector, SLOT(drag(const QPoint&)));
    connect(canvas, SIGNAL(endDrag()), toolSelector, SLOT(endDrag()));
    connect(layersWidget, SIGNAL(layerSelected(uint32_t)), &painting, SLOT(selectLayer(uint32_t)));
    connect(&painting, SIGNAL(layerAdded(uint32_t, const QString&)), layersWidget, SLOT(appendLayer(uint32_t, const QString&)));
    connect(&painting, SIGNAL(layerNameChanged(uint32_t, const QString&)), layersWidget, SLOT(changeLayerName(uint32_t, const QString&)));
    connect(&painting, SIGNAL(layerMoved(uint32_t, uint32_t)), layersWidget, SLOT(moveLayer(uint32_t, uint32_t)));

    on_layersWidget_addLayerClicked();
    toolSelector->toolButtons.buttons()[0]->click();
    colorChooser->selectColor(painting.getPenColor());
    show();
}

void ClientMainWindow::on_socket_readyRead() {
    reader.processBytes(client->readAll(), {
            {STRING_MESSAGE, [this](const QByteArray& message){
                StringMessage m(message);
                textEdit->append(m.str);
            }},
            {SET_CLIENT_NAME_MESSAGE, [this](const QByteArray& message){
                SetClientNameMessage m(message);
                setWindowTitle("Name is " + m.name);
            }},
            {PATH_MESSAGE, [this](const QByteArray& message){
                PathMessage m(message);
                painting.addStroke(m.layerId, Stroke(QColor(m.r, m.g, m.b), m.isEraser, m.points));
            }},
            {ADD_NEW_LAYER_MESSAGE, [this](const QByteArray& message){
                AddNewLayerMessage m(message);
                painting.addLayer(m.layerName);
            }},
            {RENAME_LAYER_MESSAGE, [this](const QByteArray& message){
                RenameLayerMessage m(message);
                painting.renameLayer(m.uid, m.layerName);
            }},
            {MOVE_LAYER_MESSAGE, [this](const QByteArray& message){
                MoveLayerMessage m(message);
                painting.moveLayer(m.uid, m.newPos);
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
    static TextProgress p(20);
    debug->setText(QString("linesCount: %1, paintTime: %2 ms |%3|").arg(linesCount).arg(paintTime).arg(p.get()));
}

void ClientMainWindow::strokeFinished(const Stroke& stroke) {
    uint8_t r = static_cast<uint8_t>(stroke.color.red());
    uint8_t g = static_cast<uint8_t>(stroke.color.green());
    uint8_t b = static_cast<uint8_t>(stroke.color.blue());
    uint32_t layerId = painting.getCurrentLayerId();

    sendMessage<PathMessage>(r, g, b, layerId, stroke.isEraser, stroke.polygon);
}

bool ClientMainWindow::isConnected() {
    return buttonSend->isEnabled();
}

template<typename MsgClass, typename... ArgTypes>
void ClientMainWindow::sendMessage(ArgTypes... args) {
    if(!isConnected()){
        return;
    }

    client->write(MsgClass(args...).encodeMessage());
    client->flush();
}

void ClientMainWindow::on_buttonSend_clicked(){
    if(lineEdit->text().isEmpty()){
        return;
    }

    sendMessage<StringMessage>(lineEdit->text());
    lineEdit->setText("");
}

void ClientMainWindow::on_buttonConnect_clicked(){
    buttonConnect->setEnabled(false);
    client->connectToHost("localhost", 9000);
}

void ClientMainWindow::on_colorChooser_colorSelected(const QColor& color) {
    painting.setPenColor(color);
}

void ClientMainWindow::on_layersWidget_addLayerClicked() {
    QString name = QString("New layer %1").arg(newLayerCounter++);
    painting.addLayer(name);

    sendMessage<AddNewLayerMessage>(name);
}

void ClientMainWindow::on_layersWidget_renameClicked() {
    const QString& oldName = painting.getCurrentLayer()->getName();
    QString name = QInputDialog::getText(this, "Renaming layer", "Enter layer name:", QLineEdit::Normal, oldName);
    if(name.isEmpty() || name == oldName){
        return;
    }

    painting.renameLayer(painting.getCurrentLayerId(), name);

    sendMessage<RenameLayerMessage>(painting.getCurrentLayerId(), name);
}

void ClientMainWindow::on_layersWidget_upButtonClicked(uint32_t uid) {
    int index = painting.layerIndex(uid);
    if (index == -1) {
        printf("on_layersWidget_upButtonClicked: Layer not found for uid %d\n", uid);
        return;
    }
    if (index <= 0) {
        return;
    }
    uint32_t newPos = static_cast<uint32_t>(index - 1);
    painting.moveLayer(uid, newPos);

    sendMessage<MoveLayerMessage>(uid, newPos);
}

void ClientMainWindow::on_layersWidget_downButtonClicked(uint32_t uid) {
    int index = painting.layerIndex(uid);
    if (index == -1) {
        printf("on_layersWidget_downButtonClicked: Layer not found for uid %d\n", uid);
        return;
    }
    if (index >= painting.layersCount() - 1) {
        return;
    }
    uint32_t newPos = static_cast<uint32_t>(index + 1);
    painting.moveLayer(uid, newPos);

    sendMessage<MoveLayerMessage>(uid, newPos);
}
