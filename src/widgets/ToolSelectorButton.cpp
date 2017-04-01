#include "ToolSelectorButton.h"

#include "tools/Tool.h"
#include "ToolSelectorWidget.h"

ToolSelectorButton::ToolSelectorButton(ToolSelectorWidget* parent, const QString& text, Tool* tool)
        : QPushButton(parent) {
    setCheckable(true);
    setText(text);
    connect(this, &QAbstractButton::toggled, [parent, tool](bool checked){
        if (checked) {
            parent->selectTool(tool);
        }
    });
}
