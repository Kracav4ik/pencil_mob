#pragma once

#include <QPushButton>

class ToolSelectorWidget;
class Tool;

//! Highlighted tool.
class ToolSelectorButton: public QPushButton {
public:
     //! Create new highlighted tool.
     //! \param parent
     //! \param text
     //! \param tool
    ToolSelectorButton(ToolSelectorWidget* parent, const QString& text, Tool* tool);
};


