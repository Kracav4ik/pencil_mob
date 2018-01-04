#pragma once

#include <QUndoCommand>
#include "MessageSender.h"

class Painting;

//! A base class for all editor commands.
class ClientCommand: public QUndoCommand {
protected:
    //! The editor for command to manipulate.
    Painting& painting;
    MessageSender& sender;

    template<typename MsgClass, typename... ArgTypes>
    void sendMessage(ArgTypes... args){
        sender.sendMessage<MsgClass>(args...);
    }

    //! Creates the editor command.
    //! \param painting editor for the command.
    //! \param text name of the command in "Undo ..." or "Redo ..." menus.
    ClientCommand(Painting& painting, MessageSender& sender, const QString& text);
};
