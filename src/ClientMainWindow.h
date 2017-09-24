#pragma once

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

#include "ui_pencil_mob.h"
#include "transport.h"

class ColorChooserWidget;
class ToolSelectorWidget;
class LayersWidget;

//! Client which we're starting.
class ClientMainWindow : public QMainWindow, private Ui::ClientMainWindow {
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
    //! Reader of message between client and server.
    MessageReader reader;
    //! The main part is where all the drawing takes place.
    Painting painting;
    //! Something kind of counter.
    int newLayerCounter = 1;

    //! Check connected or not.
    //! \return Answer.
    bool isConnected();

    //! Something kind of magic.
    template<typename MsgClass, typename... ArgTypes>
    void sendMessage(ArgTypes... args);

public:
    //! Starts the client.
    ClientMainWindow();

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
    uint32_t on_layersWidget_addLayerClicked();
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
};

