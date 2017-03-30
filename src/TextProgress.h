#pragma once

#include <QString>

class TextProgress{
private:
    int counter = 1;
    const int size;

public:
    TextProgress(int size) : size(qMax(size, 5)) {}

    QString get(){
        QString s(size, ' ');
        s[(counter + size - 1) % size] = '.';
        s[counter] = '.';
        s[(counter + 1) % size] = '.';
        counter = (counter + 1) % size;
        return s;
    }
};
