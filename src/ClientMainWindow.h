#pragma once

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>

#include "ui_pencil_mob.h"
#include "transport.h"

class ColorChooserWidget;
class ToolSelectorWidget;

class ClientMainWindow : public QMainWindow, private Ui::ClientMainWindow {
Q_OBJECT
private:
    QTcpSocket* client;
    ColorChooserWidget* colorChooser;
    ToolSelectorWidget* toolSelector;
    MessageReader reader;
    Painting painting;

public:
    ClientMainWindow();

    bool isConnected();

public slots:
    void on_socket_readyRead();
    void on_buttonSend_clicked();
    void on_buttonConnect_clicked();
    void on_socket_connected();
    void on_socket_error(QAbstractSocket::SocketError error);
    void on_socket_stateChanged(QAbstractSocket::SocketState state);

    void on_addLayer_clicked();

    void on_colorChooser_colorSelected(const QColor& color);

    void on_canvas_debugInfo(int linesCount, int paintTime);
    void strokeFinished(const Stroke& stroke);
};

