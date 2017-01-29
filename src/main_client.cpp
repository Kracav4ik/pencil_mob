#include <QApplication>
#include "ClientMainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    ClientMainWindow* window = new ClientMainWindow();
    return app.exec();
}
