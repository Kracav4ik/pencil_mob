#pragma once

#include <QPushButton>

class ToolSelectorWidget;
class Tool;

class ToolSelectorButton: public QPushButton {
public:
    ToolSelectorButton(ToolSelectorWidget* parent, const QString& text, Tool* tool);
};


