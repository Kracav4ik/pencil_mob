#pragma once

#include <QFrame>
#include "ui_user.h"

class UserWidget : public QFrame, private Ui::UserWidget {
Q_OBJECT

public:
    UserWidget(QWidget* parent);
    UserWidget(QWidget* parent, uint32_t uid);

public slots:
    void on_userVisible_clicked(bool b);

signals:
    void changedUserVisible(bool visible);
};
