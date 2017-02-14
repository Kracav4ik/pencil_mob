#include <QApplication>
#include <QTime>
#include "ClientMainWindow.h"

int main(int argc, char* argv[]) {
    qsrand((uint) QTime::currentTime().msecsSinceStartOfDay());
    QApplication app(argc, argv);
    ClientMainWindow* window = new ClientMainWindow();
    return app.exec();
}
