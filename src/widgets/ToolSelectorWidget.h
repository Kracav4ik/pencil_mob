#pragma once

#include <QDockWidget>

#include "tools/PenTool.h"
#include "tools/EraserTool.h"
#include "ui_tool_selector.h"

class ClientMainWindow;

class ToolSelectorWidget : public QDockWidget, private Ui::ToolSelectorWidget{
Q_OBJECT
private:
    friend class ClientMainWindow;
    PenTool penTool;
    EraserTool eraserTool;
    Tool* currentTool = nullptr;
    QButtonGroup toolButtons;

public:
    ToolSelectorWidget(QWidget* parent);

    void selectTool(Tool* tool);

signals:
    void toolSelected(Tool* tool);

public slots:
    void beginDrag();
    void drag(const QPoint& pos);
    void endDrag();
};
