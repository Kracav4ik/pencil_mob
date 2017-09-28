#include "MessagesWidget.h"

void MessagesWidget::append(const QString& str) {
    textEdit->append(str);
}

MessagesWidget::MessagesWidget(QWidget* parent)
    : QDockWidget(parent)
{
    setupUi(this);
}
