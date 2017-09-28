#pragma once

#include <QDockWidget>
#include "ui_messages.h"

//! A class for displaying messages.
class MessagesWidget: public QDockWidget, private Ui::MessagesWidget {
Q_OBJECT
public:
    //! Create widget.
    explicit MessagesWidget(QWidget* parent);

    //! Appends message to the widget.
    void append(const QString& str);
};
