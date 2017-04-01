#pragma once

#include <QDockWidget>

#include "tools/PenTool.h"
#include "ui_tool_selector.h"

class ToolSelectorWidget : public QDockWidget, private Ui::ToolSelectorWidget{
Q_OBJECT
public:
    PenTool penTool;
    Tool* currentTool = nullptr;
    ToolSelectorWidget(QWidget* parent);

    void selectTool(Tool* tool);

signals:
    void toolSelected(Tool* tool);

public slots:
    void beginDrag();
    void drag(const QPoint& pos);
    void endDrag();
};
