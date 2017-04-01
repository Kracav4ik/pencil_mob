#pragma once

#include <QDockWidget>
#include "ui_tool_selector.h"

class ToolSelectorWidget : public QDockWidget, private Ui::ToolSelectorWidget{
public:
    ToolSelectorWidget(QWidget* parent);
};
