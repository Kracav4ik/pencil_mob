#include "ClientMainWindow.h"
#include "widgets/ColorChooserWidget.h"
#include "widgets/ToolSelectorWidget.h"
#include "widgets/LayersWidget.h"
#include "widgets/ListOfVisibleUsersWidget.h"
#include "widgets/MessagesWidget.h"
#include "messages.h"
#include "TextProgress.h"
#include "Layer.h"
#include <QInputDialog>

static const float SCALE_FACTOR = powf(2, 0.25);

ClientMainWindow::ClientMainWindow()
        : client(new QTcpSocket(this))
        , colorChooser(new ColorChooserWidget(this))
        , toolSelector(new ToolSelectorWidget(this))
        , messages(new MessagesWidget(this))
        , layersWidget(new LayersWidget(this))
        , painting(this)
        , listOfVisibleUsersWidget(new ListOfVisibleUsersWidget(this, painting.getOurUserId()))
{
    client->setObjectName("socket");
    colorChooser->setObjectName("colorChooser");
    toolSelector->setObjectName("toolSelector");
    listOfVisibleUsersWidget->setObjectName("listOfVisibleUsersWidget");
    layersWidget->setObjectName("layersWidget");
    painting.setObjectName("painting");
    setupUi(this);

    addDockWidget(Qt::RightDockWidgetArea, colorChooser);
    addDockWidget(Qt::LeftDockWidgetArea, toolSelector);
    addDockWidget(Qt::RightDockWidgetArea, listOfVisibleUsersWidget);
    addDockWidget(Qt::LeftDockWidgetArea, layersWidget);
    addDockWidget(Qt::BottomDockWidgetArea, messages);
    menuView->addAction(colorChooser->toggleViewAction());
    menuView->addAction(listOfVisibleUsersWidget->toggleViewAction());
    menuView->addAction(toolSelector->toggleViewAction());
    menuView->addAction(layersWidget->toggleViewAction());
    menuView->addAction(messages->toggleViewAction());
    messages->hide();
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
    connect(canvas, SIGNAL(leftDrag(const QPoint&)), toolSelector, SLOT(drag(const QPoint&)));
    connect(canvas, SIGNAL(endDrag()), toolSelector, SLOT(endDrag()));
    connect(layersWidget, SIGNAL(layerSelected(uint32_t)), &painting, SLOT(selectLayer(uint32_t)));
    connect(listOfVisibleUsersWidget, SIGNAL(changedUserVisible(uint32_t, bool)), &painting, SLOT(changingUserVisible(uint32_t, bool)));
    connect(&painting, SIGNAL(layerAdded(uint32_t, const QString&)), layersWidget, SLOT(appendLayer(uint32_t, const QString&)));
    connect(&painting, SIGNAL(layerNameChanged(uint32_t, const QString&)), layersWidget, SLOT(changeLayerName(uint32_t, const QString&)));
    connect(&painting, SIGNAL(layerRemoved(uint32_t)), layersWidget, SLOT(deleteLayer(uint32_t)));
    connect(&painting, SIGNAL(layerSelected(uint32_t)), layersWidget, SLOT(selectLayer(uint32_t)));
    connect(&painting, SIGNAL(layerMoved(uint32_t, uint32_t)), layersWidget, SLOT(moveLayer(uint32_t, uint32_t)));
    connect(&painting, SIGNAL(userAdded(uint32_t)), listOfVisibleUsersWidget, SLOT(addUser(uint32_t)));

    on_layersWidget_addLayerClicked();
    toolSelector->toolButtons.buttons()[0]->click();
    colorChooser->selectColor(painting.getPenColor());
    show();
}

void ClientMainWindow::on_socket_readyRead() {
    reader.processBytes(client->readAll(), *this);
}

void ClientMainWindow::handleStringMessage(uint32_t user, const StringMessage& m) {
    messages->append(m.str);
}

void ClientMainWindow::handleSetClientInfoMessage(uint32_t user, const SetClientInfoMessage& m) {
    setWindowTitle("Name is " + m.name);
    // TODO: this message should not have 'uint32_t user' sender param that server uses to tell us our id
    painting.setOurUserId(user);
    listOfVisibleUsersWidget->setOurUserId(user);
}

void ClientMainWindow::handlePathMessage(uint32_t user, const PathMessage& m) {
    painting.addStroke({user, m.layerId}, Stroke(m.color, m.isEraser, m.points));
}

void ClientMainWindow::handleAddNewLayerMessage(uint32_t user, const AddNewLayerMessage& m) {
    if (!painting.containsUser(user)){
        listOfVisibleUsersWidget->addUser(user);
    }
    painting.addLayer({user, m.layerId}, m.layerName);
}

void ClientMainWindow::handleRenameLayerMessage(uint32_t user, const RenameLayerMessage& m) {
    painting.renameLayer({user, m.layerId}, m.layerName);
}

void ClientMainWindow::handleMoveLayerMessage(uint32_t user, const MoveLayerMessage& m) {
    painting.moveLayer({user, m.layerId}, m.newPos);
}

void ClientMainWindow::handleRemoveLayerMessage(uint32_t user, const RemoveLayerMessage& m) {
    painting.removeLayer({user, m.layerId});
}

void ClientMainWindow::handleCopyLayerMessage(uint32_t user, const CopyLayerMessage& m) {
    painting.copyFromLayer({m.fromUserId, m.fromLayerId}, {user, m.toLayerId});
}

void ClientMainWindow::handleLayerContentsMessage(uint32_t user, const LayerContentsMessage& m) {
    LayerId layer(user, m.layerId);
    if (painting.containsLayer(layer)) {
        qDebug() << "WTF?! error: layer " << layer << " already in painting";
        return;
    }
    painting.addLayer(layer, m.layerName);
    for (const Stroke& stroke : m.strokes) {
        painting.addStroke(layer, stroke);
    }
}

void ClientMainWindow::on_socket_connected() {
    buttonSend->setEnabled(true);
    for (const Layer* layer :painting.getLayers()) {
        sendMessage<LayerContentsMessage>(painting.uidFromLayer(layer).layer, layer->getSrokes(), layer->getName());
    }
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
    if (!painting.hasOwnLayers()) {
        return; // TODO: disable tools instead
    }
    LayerId layerId = painting.getCurrentLayerId();

    sendMessage<PathMessage>(stroke.color, layerId.layer, stroke.isEraser, stroke.polygon);
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

LayerId ClientMainWindow::on_layersWidget_addLayerClicked() {
    QString name = QString("New layer %1").arg(newLayerCounter++);
    LayerId result = painting.addLayer(name);

    sendMessage<AddNewLayerMessage>(result.layer, name);
    return result;
}

void ClientMainWindow::on_layersWidget_removeLayerClicked() {
    if (!painting.hasOwnLayers()) {
        return; // TODO: disable button instead
    }
    LayerId uid = painting.getCurrentLayerId();
    painting.removeLayer(uid);

    sendMessage<RemoveLayerMessage>(uid.layer);

    if (!painting.hasOwnLayers()) {
        on_layersWidget_addLayerClicked();
    }
}

void ClientMainWindow::on_layersWidget_duplicateLayerClicked() {
    if (!painting.hasOwnLayers()) {
        return; // TODO: disable button instead
    }
    LayerId toUid = on_layersWidget_addLayerClicked();
    LayerId fromUid = painting.getCurrentLayerId();
    painting.copyFromLayer(fromUid, toUid);

    sendMessage<CopyLayerMessage>(fromUid.user, fromUid.layer, toUid.layer);
}

void ClientMainWindow::on_layersWidget_renameClicked() {
    if (!painting.hasOwnLayers()) {
        return; // TODO: disable button instead
    }
    const QString& oldName = painting.getCurrentLayer()->getName();
    QString name = QInputDialog::getText(this, "Renaming layer", "Enter layer name:", QLineEdit::Normal, oldName);
    if(name.isEmpty() || name == oldName){
        return;
    }

    painting.renameLayer(painting.getCurrentLayerId(), name);

    sendMessage<RenameLayerMessage>(painting.getCurrentLayerId().layer, name);
}

void ClientMainWindow::on_layersWidget_upButtonClicked(uint32_t uid) {
    int index = painting.layerIndex({painting.getOurUserId(), uid});
    if (index == -1) {
        qDebug() << "on_layersWidget_upButtonClicked: Layer not found for uid" << uid;
        return;
    }
    if (index <= 0) {
        return;
    }
    uint32_t newPos = static_cast<uint32_t>(index - 1);
    painting.moveLayer({painting.getOurUserId(), uid}, newPos);

    sendMessage<MoveLayerMessage>(uid, newPos);
}

void ClientMainWindow::on_layersWidget_downButtonClicked(uint32_t uid) {
    int index = painting.layerIndex({painting.getOurUserId(), uid});
    if (index == -1) {
        qDebug() << "on_layersWidget_downButtonClicked: Layer not found for uid" << uid;
        return;
    }
    if (index >= painting.layersCount() - 1) {
        return;
    }
    uint32_t newPos = static_cast<uint32_t>(index + 1);
    painting.moveLayer({painting.getOurUserId(), uid}, newPos);

    sendMessage<MoveLayerMessage>(uid, newPos);
}

void ClientMainWindow::on_canvas_mouseWheel(float delta) {
    if (delta > 0) {
        canvas->zoomCamera(SCALE_FACTOR);
    } else {
        canvas->zoomCamera(1/SCALE_FACTOR);
    }
    canvas->update();
}

void ClientMainWindow::on_canvas_rightDrag(const QPointF& delta) {
    canvas->moveCamera(delta);
    canvas->update();
}

void ClientMainWindow::on_canvas_zoomChanged(float z) {
    QString text;
    if (z >= 1) {
        text = QString("%1%").arg(int(z*100));
    } else {
        text = QString("%1%").arg(z*100, 0, 'g', 3);
    }
    zoom->setText(text);
}

void ClientMainWindow::on_resetZoom_clicked() {
    canvas->resetZoomCamera();
    canvas->update();
}
