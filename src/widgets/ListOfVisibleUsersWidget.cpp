#include "ListOfVisibleUsersWidget.h"
#include "UserWidget.h"

ListOfVisibleUsersWidget::ListOfVisibleUsersWidget(QWidget* parent): QDockWidget(parent) {
    setupUi(this);
    addUser(0);
}

void ListOfVisibleUsersWidget::addUser(uint32_t uid) {
    UserWidget* userWidget = uid? new UserWidget(this, uid): new UserWidget(this);
    QVBoxLayout* boxLayout = reinterpret_cast<QVBoxLayout*>(userVisible->layout());
    boxLayout->insertWidget(boxLayout->count() - 1, userWidget);

    connect(userWidget, &UserWidget::changedUserVisible, [this, uid](bool visible) {
        emit changedUserVisible(uid, visible);
    });
}
