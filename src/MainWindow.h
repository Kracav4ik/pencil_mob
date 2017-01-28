#pragma once

#include <QMainWindow>

#include "ui_pencil_mob.h"



class MainWindow : public QMainWindow, private Ui::MainWindow {
        Q_OBJECT
    public:
        MainWindow() {
            setupUi(this);
            show();
        }

    public slots:
        void on_pushButton_clicked();

};

