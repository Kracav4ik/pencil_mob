#pragma once

#include <QUndoCommand>
#include "ClientMainWindow.h"       

class Painting;

//! A base class for all editor commands.
class ClientCommand: public QUndoCommand {
protected:
    //! The editor for command to manipulate.
    Painting& painting;
    ClientMainWindow& main;
    template<typename MsgClass, typename... ArgTypes>
    void sendMessage(ArgTypes... args){
        main.sendMessage<MsgClass>(args...);
    }

    //! Creates the editor command.
    //! \param painting editor for the command.
    //! \param text name of the command in "Undo ..." or "Redo ..." menus.
    ClientCommand(Painting& painting, ClientMainWindow& main, const QString& text);
};
