#include "UserWidget.h"

UserWidget::UserWidget(QWidget* parent) : QFrame(parent) {
    setupUi(this);
    userName->setText(QString("Ours layers"));
}

UserWidget::UserWidget(QWidget* parent, uint32_t uid) : QFrame(parent) {
    setupUi(this);
    userName->setText(QString("User #%1").arg(uid));
}

void UserWidget::on_userVisible_clicked(bool b) {
    emit changedUserVisible(b);
}
