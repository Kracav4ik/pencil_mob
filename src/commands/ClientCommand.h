#pragma once

#include <QUndoCommand>

class Painting;

//! A base class for all editor commands.
class ClientCommand: public QUndoCommand {
protected:
    //! The editor for command to manipulate.
    Painting& painting;

    //! Creates the editor command.
    //! \param painting editor for the command.
    //! \param text name of the command in "Undo ..." or "Redo ..." menus.
    ClientCommand(Painting& painting, const QString& text);
};
