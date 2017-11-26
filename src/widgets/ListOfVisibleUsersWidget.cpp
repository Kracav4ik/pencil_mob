#include "ListOfVisibleUsersWidget.h"
#include "UserWidget.h"

ListOfVisibleUsersWidget::ListOfVisibleUsersWidget(QWidget* parent, uint32_t ourUserId): QDockWidget(parent), ourUserId(ourUserId) {
    setupUi(this);
}

void ListOfVisibleUsersWidget::addUser(uint32_t uid) {
    UserWidget* userWidget;
    if (uid != ourUserId) {
        userWidget = new UserWidget(this, uid);
    } else {
        userWidget = new UserWidget(this);
    }
    QVBoxLayout* boxLayout = reinterpret_cast<QVBoxLayout*>(userVisible->layout());
    boxLayout->insertWidget(boxLayout->count() - 1, userWidget);

    // TODO: this is kinda ugly
    if (uid != ourUserId) {
        connect(userWidget, &UserWidget::changedUserVisible, [this, uid](bool visible) {
            emit changedUserVisible(uid, visible);
        });
    } else {
        connect(userWidget, &UserWidget::changedUserVisible, [this](bool visible) {
            emit changedUserVisible(ourUserId, visible);
        });
    }
}

void ListOfVisibleUsersWidget::setOurUserId(uint32_t newOurUserId) {
    ourUserId = newOurUserId;
}
