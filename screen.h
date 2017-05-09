#ifndef SCREEN_H
#define SCREEN_H

#include <QObject>
#include "settings.h"

class Screen : public QObject
{
    Q_OBJECT

    int id;
    int dimension_m;
    const char * surface;

    //+ borders of source:
    double xLower, xUpper;
    double yLower, yUpper;
    double zLower, zUpper;
    //-

    // кол-во детекторов по оси X (пока равно по оси Y)
    long int numberOfCellsX;
    // кол-во детекторов по оси Y (пока равно по оси X)
    long int numberOfCellsY;

public:
    explicit Screen(QObject *parent = 0);

    int getId() const;
    void setId(int value);
};

#endif // SCREEN_H
