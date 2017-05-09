#include "screen.h"



Screen::Screen(QObject *parent) :
    QObject(parent)
{
}

int Screen::getId() const
{
    return id;
}

void Screen::setId(int value)
{
    id = value;
}
