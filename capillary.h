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
    float100et accuracy;
    float100et accuracyPhysics;
    QByteArray comment;

public:
    Capillary(int sourceId);
    ~Capillary();
    Surface *getSurface() const;
    Surface *getLowerEdge() const;
    Surface *getUpperEdge() const;
    const float100et *getAccuracy() const;
    const float100et *getAccuracyPhysics() const;
};

#endif // CAPILLARY_H
