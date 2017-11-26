#pragma once

#include <QDockWidget>
#include "ui_users.h"

class UserWidget;

class ListOfVisibleUsersWidget : public QDockWidget, private Ui::ListOfVisibleUsersWidget {
Q_OBJECT
private:
    uint32_t ourUserId;

public:
    explicit ListOfVisibleUsersWidget(QWidget* parent, uint32_t ourUserId);

    void setOurUserId(uint32_t newOurUserId);

public slots:
    void addUser(uint32_t uid);

signals:
    void changedUserVisible(uint32_t uid, bool visible);
};
