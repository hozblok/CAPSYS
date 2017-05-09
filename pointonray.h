#ifndef POINTONRAY_H
#define POINTONRAY_H

#include "ray.h"
#include "settings.h"
#include "screen.h"

class Ray; //??

class PointOnRay
{
    int id;
    Ray *ray;
    int numberInRay;
    REAL x, y, z;
    REAL xDirectingVec, yDirectingVec, zDirectingVec;
    REAL angle;
    bool onSource, onScreen;
    Screen *screen;
    bool modified;

public:
    PointOnRay(Ray *tRay, bool tOnSource = false, bool tOnScreen = false, Screen *tScreen = 0);
    ~PointOnRay();
    const REAL *getX() const;
    void setX(const REAL &value);
    const REAL *getY() const;
    void setY(const REAL &value);
    const REAL *getZ() const;
    void setZ(const REAL &value);
    const REAL *getXDirectingVec() const;
    void setXDirectingVec(const REAL &value);
    const REAL *getYDirectingVec() const;
    void setYDirectingVec(const REAL &value);
    const REAL *getZDirectingVec() const;
    void setZDirectingVec(const REAL &value);
    const REAL *getAngle() const;
    void setAngle(const REAL &value);
    bool getModified() const;
    void setModified(bool value);
    bool getOnScreen() const;
    void setOnScreen(bool value);

    int getNumberInRay() const;


};

#endif // POINTONRAY_H
