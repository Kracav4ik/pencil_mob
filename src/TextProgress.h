#pragma once

#include <QString>

//! Process bar for prettiness.
class TextProgress{
private:
    //! Counter of dots position.
    int counter = 1;
    //! Maximum chars size.
    const int size;

public:
    //!  Create TextProgress.
    //! \param size Maximum chars size.
    explicit TextProgress(int size) : size(qMax(size, 5)) {}

    //! Procceses chars
    QString get(){
        QString s(size, ' ');
        s[(counter + size - 1) % size] = '.';
        s[counter] = '.';
        s[(counter + 1) % size] = '.';
        counter = (counter + 1) % size;
        return s;
    }
};
