#include "ToolSelectorWidget.h"

#include "ToolSelectorButton.h"
#include <QVBoxLayout>

ToolSelectorWidget::ToolSelectorWidget(QWidget* parent)
        : QDockWidget(parent), toolButtons(this) {
    setupUi(this);
    QHBoxLayout* layout = new QHBoxLayout();
    ToolSelectorButton* button;

    button = new ToolSelectorButton(this, QIcon("../icons/Pen tool.png"), &penTool);
    toolButtons.addButton(button);
    layout->addWidget(button);

    button = new ToolSelectorButton(this, QIcon("../icons/Eraser tool.png"), &eraserTool);
    toolButtons.addButton(button);
    layout->addWidget(button);

    layout->addSpacerItem(new QSpacerItem(100, 0));
    dockWidgetContents->setLayout(layout);
}

void ToolSelectorWidget::selectTool(Tool* tool) {
    if (currentTool != tool) {
        currentTool = tool;
        emit toolSelected(currentTool);
    }
}

void ToolSelectorWidget::beginDrag(const QPoint& pos) {
    if (currentTool){
        currentTool->beginDrag(pos);
    }
}

void ToolSelectorWidget::drag(const QPoint& pos) {
    if (currentTool){
        currentTool->drag(pos);
    }
}

void ToolSelectorWidget::endDrag() {
    if (currentTool){
        currentTool->endDrag();
    }
}

QList<Tool*> ToolSelectorWidget::allTools() {
    return {&penTool, &eraserTool};
}
