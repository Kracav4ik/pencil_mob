#pragma once

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUndoStack>
#include <QTimer>

#include "ui_pencil_mob.h"
#include "transport.h"
#include "MessageHandler.h"
#include "MessageSender.h"

class ColorChooserWidget;
class ToolSelectorWidget;
class LayersWidget;
class MessagesWidget;
class ListOfVisibleUsersWidget;
class ClientCommand;

//! Client which we're starting.
class ClientMainWindow : public QMainWindow, private Ui::ClientMainWindow, private MessageHandler, private MessageSender {
Q_OBJECT
private:
    //! Socket pointer for connect to server.
    QTcpSocket* client;
    //! For choose a color
    ColorChooserWidget* colorChooser;
    //! For select tool.
    ToolSelectorWidget* toolSelector;
    //! Widget with layers.
    LayersWidget* layersWidget;
    //! Widget with messages.
    MessagesWidget* messages;
    //! The path to current opened file, null string if no file is open.
    QString path;
    //! Reader of message between client and server.
    ClientMessageReader reader;
    //! The main part is where all the drawing takes place.
    Painting painting;
    //! Something kind of counter.
    int newLayerCounter = 1;
    QTimer timer;
    QFile recent;

    ListOfVisibleUsersWidget* listOfVisibleUsersWidget;
    //! Undo stack for editor commands.
    QUndoStack undoStack;
    //! Check connected or not.
    //! \return Answer.
    bool isConnected() const override;

    void writeMessage(const QByteArray& data) override;

    //! Returns the path to levels directory ("data/levels").
    static QString getImagesDir();
    //! Saves level to specified path and sets path as current if there were no errors.
    //! \param savePath specified path.
    void doSaveLayers(const QString& savePath);
    void open(const QString& openPath);

    void handleStringMessage(uint32_t user, const StringMessage& m) override;
    void handleSetClientInfoMessage(uint32_t user, const SetClientInfoMessage& m) override;
    void handlePathMessage(uint32_t user, const PathMessage& m) override;
    void handleAddNewLayerMessage(uint32_t user, const AddNewLayerMessage& m) override;
    void handleRenameLayerMessage(uint32_t user, const RenameLayerMessage& m) override;
    void handleMoveLayerMessage(uint32_t user, const MoveLayerMessage& m) override;
    void handleRemoveLayerMessage(uint32_t user, const RemoveLayerMessage& m) override;
    void handleCopyLayerMessage(uint32_t user, const CopyLayerMessage& m) override;
    void handleLayerContentsMessage(uint32_t user, const LayerContentsMessage& m) override;
    void handleRemoveLastStrokeMessage(uint32_t user, const RemoveLastStrokeMessage& m) override;

public:
    //! Starts the client.
    ClientMainWindow();

    //! Push the command to the undo stack.
    void pushCommand(ClientCommand& command);
    //! Moving the layer from given uid down (if the second parameter true) or up (if the second parameter false).
    //! \param uid Layer uid.
    //! \param down The parameter that allows you to change direction of moving layer(up or down).
    void moveDown(uint32_t uid, bool down);

protected:
    void closeEvent(QCloseEvent* event) override;

public slots:
    //! Connects the client to the server.
    void on_socket_readyRead();
    //! If the client connected to the server.
    void on_socket_connected();
    //! If an error occurred while connecting.
    //! \param error The socket error
    void on_socket_error(QAbstractSocket::SocketError error);
    //! If the socket state was be changed.
    //! \param state The socket state.
    void on_socket_stateChanged(QAbstractSocket::SocketState state);

    //! The debug inforation from the canvas.
    void on_canvas_debugInfo(int linesCount, int paintTime);
    //! Send the stroke to server.
    //! \param stroke The stroke that was sended.
    void strokeFinished(const Stroke& stroke);
    //! If button send clicked.
    void on_buttonSend_clicked();
    //! If button connect clicked.
    void on_buttonConnect_clicked();
    //! Selects the pen color from politra.
    //! \param color The new color from politra.
    void on_colorChooser_colorSelected(const QColor& color);

    //! Adds a new layer to painting from layersWidget.
    //! \return The layer uid.
    LayerId on_layersWidget_addLayerClicked();
    //! Renames the current layer on widget.
    void on_layersWidget_renameClicked();
    //! Removes the current layer on widget.
    void on_layersWidget_removeLayerClicked();
    //! Duplicates the current layer on widget.
    void on_layersWidget_duplicateLayerClicked();
    //! Move the current layer up in z-order by one.
    void on_layersWidget_upButtonClicked(uint32_t uid);
    //! Move the current layer down in z-order by one.
    void on_layersWidget_downButtonClicked(uint32_t uid);
    void on_canvas_mouseWheel(float delta);
    void on_canvas_rightDrag(const QPointF& delta);
    void on_canvas_zoomChanged(float z);
    void on_resetZoom_clicked();
    void on_zoomIn_clicked();
    void on_zoomOut_clicked();

private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAs_triggered();
    void updateTitle();
    void resave();
    void densityChanged(int a);
    void setCurrentTool(Tool* tool);
};

