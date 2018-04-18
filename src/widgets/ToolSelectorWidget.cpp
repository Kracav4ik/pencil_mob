#include "ToolSelectorWidget.h"

#include "ToolSelectorButton.h"
#include <QVBoxLayout>

ToolSelectorWidget::ToolSelectorWidget(QWidget* parent)
        : QDockWidget(parent), toolButtons(this) {
    setupUi(this);
    ToolSelectorButton* button;

    button = new ToolSelectorButton(this, QIcon("../icons/Pen tool.png"), &penTool);
    toolButtons.addButton(button);
    container->layout()->addWidget(button);

    button = new ToolSelectorButton(this, QIcon("../icons/Eraser tool.png"), &eraserTool);
    toolButtons.addButton(button);
    container->layout()->addWidget(button);

    qobject_cast<QHBoxLayout*>(container->layout())->addSpacerItem(new QSpacerItem(100, 0));
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
