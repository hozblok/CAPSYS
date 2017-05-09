#ifndef RAY_H
#define RAY_H

#include "capillary.h"
#include "pointonray.h"
#include "source.h"

class Source;
class Capillary;
class PointOnRay;

class Ray
{
    int id;
    Source *source;
    Capillary *capillary;
    REAL xSource, ySource, zSource;
    REAL phi, theta;
    REAL xDirectingVec, yDirectingVec, zDirectingVec;
    bool captured;
    bool reachedScreen;
    PointOnRay *currentPoint;
    PointOnRay *nextPoint;

    int setXYZSource();
    int setPhiThetaDirVec();
    int createPoint();

public:
    int recreateNextPoint();
    int acceptNextPoint();
    Ray(Source * tSource, int tId = 0);
    ~Ray();
    int getId() const;
    REAL getXSource() const;
    REAL getYSource() const;
    REAL getZSource() const;
    REAL getXDirectingVec() const;
    REAL getYDirectingVec() const;
    REAL getZDirectingVec() const;
    PointOnRay *getCurrentPoint() const;
    PointOnRay *getNextPoint() const;
    bool getCaptured() const;
    void setCaptured(bool value);
    Capillary *getCapillary() const;
    void setCapillary(Capillary *value);
};

#endif // RAY_H
