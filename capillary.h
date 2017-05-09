#ifndef CAPILLARY_H
#define CAPILLARY_H

#include <QSqlQuery>
#include "settings.h"
#include "surface.h"

class Surface;

class Capillary
{
    int id;
    int dimensionM; // I would like mkm (10e-6 m)
    Surface *surface;
    Surface *lowerEdge;
    Surface *upperEdge;
    REAL accuracy;
    REAL accuracyPhysics;
    QByteArray comment;

public:
    Capillary(int sourceId);
    ~Capillary();
    Surface *getSurface() const;
    Surface *getLowerEdge() const;
    Surface *getUpperEdge() const;
    const REAL *getAccuracy() const;
    const REAL *getAccuracyPhysics() const;
};

#endif // CAPILLARY_H
