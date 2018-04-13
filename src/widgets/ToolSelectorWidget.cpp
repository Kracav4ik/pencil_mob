#include "ToolSelectorWidget.h"

#include "ToolSelectorButton.h"
#include <QVBoxLayout>

ToolSelectorWidget::ToolSelectorWidget(QWidget* parent)
        : QDockWidget(parent), toolButtons(this) {
    setupUi(this);
    QVBoxLayout* layout = new QVBoxLayout();
    ToolSelectorButton* button;

    button = new ToolSelectorButton(this, "No tool", nullptr);
    toolButtons.addButton(button);
    layout->addWidget(button);

    button = new ToolSelectorButton(this, "Pen tool", &penTool);
    toolButtons.addButton(button);
    layout->addWidget(button);

    button = new ToolSelectorButton(this, "EraserTool", &eraserTool);
    toolButtons.addButton(button);
    layout->addWidget(button);

    layout->addSpacerItem(new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
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
