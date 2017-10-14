#pragma once

#include <QDockWidget>
#include "ui_users.h"

class UserWidget;

class ListOfVisibleUsersWidget : public QDockWidget, private Ui::ListOfVisibleUsersWidget {
Q_OBJECT

public:
    explicit ListOfVisibleUsersWidget(QWidget* parent);

    void addUser(uint32_t uid);

signals:
    void changedUserVisible(uint32_t uid, bool visible);
};
