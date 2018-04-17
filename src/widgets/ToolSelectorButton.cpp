#include "ToolSelectorButton.h"

#include "tools/Tool.h"
#include "ToolSelectorWidget.h"

ToolSelectorButton::ToolSelectorButton(ToolSelectorWidget* parent, const QIcon& icon, Tool* tool)
        : QPushButton(parent) {
    setCheckable(true);
    setIcon(icon);
    connect(this, &QAbstractButton::toggled, [parent, tool](bool checked){
        if (checked) {
            parent->selectTool(tool);
        }
    });
}
