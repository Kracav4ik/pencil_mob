#pragma once

#include <QDockWidget>

#include "tools/PenTool.h"
#include "tools/EraserTool.h"
#include "ui_tool_selector.h"

class ClientMainWindow;

class ToolSelectorWidget : public QDockWidget, private Ui::ToolSelectorWidget{
Q_OBJECT
private:
    friend class ClientMainWindow; // TODO: unfriend
    PenTool penTool;
    EraserTool eraserTool;
    Tool* currentTool = nullptr;
    QButtonGroup toolButtons;

public:
    explicit ToolSelectorWidget(QWidget* parent);

    void selectTool(Tool* tool);

    QList<Tool*> allTools();

signals:
    void toolSelected(Tool* tool);

public slots:
    void beginDrag();
    void drag(const QPoint& pos);
    void endDrag();
};
