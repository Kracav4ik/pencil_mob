#include "ClientMainWindow.h"
#include "widgets/ColorChooserWidget.h"
#include "widgets/ToolSelectorWidget.h"
#include "widgets/LayersWidget.h"
#include "widgets/ListOfVisibleUsersWidget.h"
#include "widgets/MessagesWidget.h"
#include "messages.h"
#include "TextProgress.h"
#include "Layer.h"
#include "commands/AddLayerCommand.h"
#include "commands/RemoveLayerCommand.h"
#include "commands/DuplicateLayerCommand.h"
#include "commands/RenameLayerCommand.h"
#include "commands/MoveLayerCommand.h"
#include "commands/AddStrokeToCurrentLayerCommand.h"
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>

static const float SCALE_FACTOR = powf(2, 0.25);

const QString APP_NAME = " - Pencil mob";

ClientMainWindow::ClientMainWindow()
        : client(new QTcpSocket(this))
        , colorChooser(new ColorChooserWidget(this))
        , toolSelector(new ToolSelectorWidget(this))
        , messages(new MessagesWidget(this))
        , layersWidget(new LayersWidget(this))
        , painting(this)
        , listOfVisibleUsersWidget(new ListOfVisibleUsersWidget(this, painting.getOurUserId()))
{
    // create UI elements and autoconnect signals
    client->setObjectName("socket");
    colorChooser->setObjectName("colorChooser");
    toolSelector->setObjectName("toolSelector");
    listOfVisibleUsersWidget->setObjectName("listOfVisibleUsersWidget");
    layersWidget->setObjectName("layersWidget");
    painting.setObjectName("painting");
    setupUi(this);

    // dock widgets
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
    connect(&painting, &Painting::changed, canvas, &CanvasWidget::updateWidget);
    connect(&painting, &Painting::penColorChanged, &toolSelector->penTool, &PenTool::setPenColor);
    for (Tool* tool : toolSelector->allTools()) {
        connect(tool, &Tool::strokeFinished, this, &strokeFinished);
        connect(tool, &Tool::needRepaint, canvas, &CanvasWidget::updateWidget);
    }
    connect(toolSelector, &ToolSelectorWidget::toolSelected, &painting, &Painting::setCurrentTool);
    connect(canvas, &CanvasWidget::beginDrag, toolSelector, &ToolSelectorWidget::beginDrag);
    connect(canvas, &CanvasWidget::leftDrag, toolSelector, &ToolSelectorWidget::drag);
    connect(canvas, &CanvasWidget::endDrag, toolSelector, &ToolSelectorWidget::endDrag);
    connect(layersWidget, &LayersWidget::layerSelected, &painting, &Painting::selectLayer);
    connect(layersWidget, &LayersWidget::layerVisibleChanged, &painting, &Painting::changeLayerVisible);
    connect(listOfVisibleUsersWidget, &ListOfVisibleUsersWidget::changedUserVisible, &painting, &Painting::changingUserVisible);
    connect(&painting, &Painting::layerAdded, layersWidget, &LayersWidget::appendLayer);
    connect(&painting, &Painting::ownLayerAdded, layersWidget, &LayersWidget::appendOwnLayer);
    connect(&painting, &Painting::ownLayerNameChanged, layersWidget, &LayersWidget::changeLayerName);
    connect(&painting, &Painting::layerNameChanged, layersWidget, &LayersWidget::changeOwnLayerName);
    connect(&painting, &Painting::ownLayerRemoved, layersWidget, &LayersWidget::deleteOwnLayer);
    connect(&painting, &Painting::layerRemoved, layersWidget, &LayersWidget::deleteLayer);
    connect(&painting, &Painting::layerSelected, layersWidget, &LayersWidget::selectLayer);
    connect(&painting, &Painting::ownLayerMoved, layersWidget, &LayersWidget::moveOwnLayer);
    connect(&painting, &Painting::layerMoved, layersWidget, &LayersWidget::moveLayer);
    connect(&painting, &Painting::userAdded, listOfVisibleUsersWidget, &ListOfVisibleUsersWidget::addUser);
    connect(&undoStack, &QUndoStack::cleanChanged, this, &updateTitle);

    // undo/redo menu items
    QAction* undoAction = undoStack.createUndoAction(this);
    undoAction->setShortcut(QKeySequence("Ctrl+Z"));
    menuView->addAction(undoAction);
    QAction* redoAction = undoStack.createRedoAction(this);
    redoAction->setShortcut(QKeySequence("Ctrl+Shift+Z"));
    menuView->addAction(redoAction);

    connect(&undoStack, &QUndoStack::indexChanged, canvas, &CanvasWidget::updateWidget);

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

void ClientMainWindow::handleRemoveLastStrokeMessage(uint32_t user, const RemoveLastStrokeMessage& m) {
    painting.removeLastStroke({user, m.layerId});
}

void ClientMainWindow::handleAddNewLayerMessage(uint32_t user, const AddNewLayerMessage& m) {
    if (!painting.containsUser(user)){
        listOfVisibleUsersWidget->addUser(user);
    }
    painting.addLayer({user, m.layerId}, m.layerName);
    qDebug() << "got add" << LayerId{user, m.layerId};
}

void ClientMainWindow::handleRenameLayerMessage(uint32_t user, const RenameLayerMessage& m) {
    painting.renameLayer({user, m.layerId}, m.layerName);
}

void ClientMainWindow::handleMoveLayerMessage(uint32_t user, const MoveLayerMessage& m) {
    painting.moveLayer({user, m.layerId}, m.newPos);
}

void ClientMainWindow::handleRemoveLayerMessage(uint32_t user, const RemoveLayerMessage& m) {
    painting.removeLayer({user, m.layerId});
    qDebug() << "got remove" << LayerId{user, m.layerId};
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
    painting.addLayer(layer, m.layerName, user < painting.getOurUserId());
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
            updateTitle();
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

    AddStrokeToCurrentLayerCommand* cmd = new AddStrokeToCurrentLayerCommand(painting, *this, stroke);
    pushCommand(*cmd);
}

bool ClientMainWindow::isConnected() const {
    return buttonSend->isEnabled();
}

void ClientMainWindow::writeMessage(const QByteArray& data) {
    client->write(data);
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
    AddLayerCommand* cmd = new AddLayerCommand(painting, *this, name, painting.getNextLayerUid());
    pushCommand(*cmd);
    return cmd->getLayerId();
}

void ClientMainWindow::on_layersWidget_removeLayerClicked() {
    if (!painting.hasOwnLayers()) {
        return; // TODO: disable button instead
    }

    pushCommand(RemoveLayerCommand::create(painting, *this, painting.getCurrentLayerId().layer));

    if (!painting.hasOwnLayers()) {
        on_layersWidget_addLayerClicked();
    }
}

void ClientMainWindow::on_layersWidget_duplicateLayerClicked() {
    if (!painting.hasOwnLayers()) {
        return; // TODO: disable button instead
    }
    LayerId fromUid = painting.getCurrentLayerId();

    DuplicateLayerCommand* cmd = new DuplicateLayerCommand(painting, *this, painting.getNextLayerUid(), fromUid);
    pushCommand(*cmd);
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

    RenameLayerCommand* cmd = new RenameLayerCommand(painting, *this, oldName, painting.getCurrentLayerId().layer, name);
    pushCommand(*cmd);
}

void ClientMainWindow::on_layersWidget_upButtonClicked(uint32_t uid) {
    moveDown(uid, false);
}

void ClientMainWindow::moveDown(uint32_t uid, bool down) {
    LayerId layerId(painting.getOurUserId(), uid);
    int index = painting.layerIndex(layerId);
    if (index == -1) {
        qDebug() << "moveUp: Layer not found for uid" << uid;
        return;
    }
    if (down) {
        if (index == painting.layersCount() - 1) {
            return;
        }
    } else {
        if (index == 0) {
            return;
        }
    }
    uint32_t newPos;
    if (down) {
        newPos = static_cast<uint32_t>(index + 1);
    } else {
        newPos = static_cast<uint32_t>(index - 1);
    }
    MoveLayerCommand* cmd = new MoveLayerCommand(painting, *this, uid, static_cast<uint32_t>(index), newPos);
    pushCommand(*cmd);
}

void ClientMainWindow::on_layersWidget_downButtonClicked(uint32_t uid) {
    moveDown(uid, true);
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

bool loadLayers(Painting& p, const QString& path, QString* errorMsg) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)){
        if (errorMsg) {
            *errorMsg = QString("Couldn't open file \"%1\" for reading.").arg(path);
        }
        return false;
    }

    p.read(QJsonDocument::fromJson(file.readAll()).object());
    return true;

}

QString ClientMainWindow::getImagesDir() {
    return QCoreApplication::applicationDirPath() + "/../data/images";
}

void ClientMainWindow::on_actionOpen_triggered() {
    QString openPath = QFileDialog::getOpenFileName(this, "Open image", getImagesDir(), "JSON files (*.json)");
    if (openPath.isNull()){
        return;
    }

    QString errorMsg;
    if (!loadLayers(painting, openPath, &errorMsg)) {
        QMessageBox::critical(this, "Error opening image", errorMsg);
        return;
    }
    path = openPath;
    updateTitle();
    undoStack.clear();
    canvas->update();
}

void ClientMainWindow::on_actionSave_triggered() {
    if (path.isNull()) {
        on_actionSaveAs_triggered();
    } else {
        doSaveLayers(path);
    }
}

void ClientMainWindow::on_actionSaveAs_triggered() {
    QString savePath = QFileDialog::getSaveFileName(this, "Save layers as", getImagesDir(), "JSON files (*.json)");
    doSaveLayers(savePath);
}

bool saveLayers(const Painting& p, const QString& path, QString* errorMsg) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)){
        if (errorMsg) {
            *errorMsg = QString("Couldn't open file \"%1\" for writing.").arg(path);
        }
        return false;
    }

    // root object to be filled.
    QJsonObject root;
    p.write(root);
    file.write(QJsonDocument(root).toJson(QJsonDocument::Compact));
    return true;
}


void ClientMainWindow::doSaveLayers(const QString& savePath) {
    if (savePath.isNull()){
        return;
    }

    QString errorMsg;
    if (!saveLayers(painting, savePath, &errorMsg)) {
        QMessageBox::critical(this, "Error saving layers", errorMsg);
        return;
    }
    path = savePath;
    updateTitle();
    undoStack.setClean();
}

void ClientMainWindow::updateTitle() {
    if (isConnected()){
        return;
    }
    if (path.isNull()) {
        setWindowTitle("* New" + APP_NAME);
        return;
    }
    QString title = QDir(getImagesDir()).relativeFilePath(path) + APP_NAME;
    if (!undoStack.isClean()) {
        title = "* " + title;
    }
    setWindowTitle(title);
}

void ClientMainWindow::pushCommand(ClientCommand& command) {
    undoStack.push(&command);
}
