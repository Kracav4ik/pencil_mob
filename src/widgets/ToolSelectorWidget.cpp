#include "ToolSelectorWidget.h"

ToolSelectorWidget::ToolSelectorWidget(QWidget* parent)
        : QDockWidget(parent) {
    setupUi(this);
}

void ToolSelectorWidget::selectTool(Tool* tool) {
    currentTool = tool;
    emit toolSelected(currentTool);
}

void ToolSelectorWidget::beginDrag() {
    if (currentTool){
        currentTool->beginDrag();
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
