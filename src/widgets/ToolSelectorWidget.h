#pragma once

#include <QDockWidget>

#include "tools/PenTool.h"
#include "tools/EraserTool.h"
#include "ui_tool_selector.h"

class ClientMainWindow;

//! The widget of tools.
class ToolSelectorWidget : public QDockWidget, private Ui::ToolSelectorWidget{
Q_OBJECT
private:
    //! Because fuck you thats why.
    friend class ClientMainWindow; // TODO: unfriend
    //! Pen. The main tool for drawing.
    PenTool penTool;
    //! Eraser. I think it will not be difficult to guess what this tool is doing. That's right, it's erasing.
    EraserTool eraserTool;
    //! A pointer to the tool we're drawing now.
    Tool* currentTool = nullptr;
    //! Used to keep only one button selected.
    QButtonGroup toolButtons;

public:
    //! Create widget.
    //! \param parent parent widget.
    explicit ToolSelectorWidget(QWidget* parent);

    //! Selects the highlighted tool.
    //! \param tool A new tool that will be selected.
    void selectTool(Tool* tool);

    //! List of tools.
    QList<Tool*> allTools();

signals:
    //! Tells the client which tool is selected.
    //! \param tool The selected tool.
    void toolSelected(Tool* tool);

public slots:
    //! We catch the beginning of the mouse drag.
    void beginDrag(const QPoint& pos);

    //! We catch the drag of the mouse.
    //! \param pos mouse position.
    void drag(const QPoint& pos);

    //! We catch the ending of the mouse drag.
    void endDrag();
};
