#include <QApplication>
#include "ServerMainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    ServerMainWindow* window = new ServerMainWindow();
    return app.exec();
}
