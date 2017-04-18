#include "ClientMainWindow.h"
#include "transport.h"
#include "enums.h"
#include "widgets/ColorChooserWidget.h"
#include "widgets/ToolSelectorWidget.h"
#include "widgets/LayersWidget.h"
#include "messages.h"
#include "TextProgress.h"

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
    connect(layersWidget->addLayer, SIGNAL(clicked()), this, SLOT(addLayerSocket())); // TODO: we should not know innards of layersWidget
    connect(layersWidget, SIGNAL(selectedLayer(uint32_t)), &painting, SLOT(selectLayer(uint32_t)));

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
                painting.selectLayer(m.layerId);
                painting.addStroke(Stroke(QColor(m.r, m.g, m.b), m.isEraser, m.points));
            }},
            {ADD_NEW_LAYER_MESSAGE, [this](const QByteArray& message){
                addLayerExt();
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
    if(!isConnected()){
        return;
    }
    uint8_t r = (uint8_t) stroke.color.red();
    uint8_t g = (uint8_t) stroke.color.green();
    uint8_t b = (uint8_t) stroke.color.blue();
    uint32_t layerId = painting.getCurrentLayerId();
    client->write(PathMessage(r, g, b, layerId, stroke.isEraser, stroke.polygon).encodeMessage());
    client->flush();
}

bool ClientMainWindow::isConnected() {
    return buttonSend->isEnabled();
}

void ClientMainWindow::on_colorChooser_colorSelected(const QColor& color) {
    painting.setPenColor(color);
}

void ClientMainWindow::addLayerExt() {
    painting.addLayer();

    layersWidget->mmm(layersWidget->verticalLayout, "a"); // TODO: we should not know innards of layersWidget

    layersWidget->verticalLayout->addSpacerItem(new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
    layersWidget->layers->setLayout(layersWidget->verticalLayout);
}

void ClientMainWindow::addLayerSocket() {
    addLayerExt();
    if(!isConnected()){
        return;
    }
    client->write(AddNewLayerMessage(QString("")).encodeMessage()); // TODO: send actual name
    client->flush();
}
