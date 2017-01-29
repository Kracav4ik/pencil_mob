#include "MainWindow.h"
void MainWindow::on_pushButton_clicked(){
    if(lineEdit->text().isEmpty()){
        return;
    }
    textEdit->setText(textEdit->toPlainText() + "\n" + lineEdit->text());
    lineEdit->setText("");
}

void MainWindow::acceptConnection() {
    printf("NEW CONNECTION ACCEPTED!!! \n");
}
