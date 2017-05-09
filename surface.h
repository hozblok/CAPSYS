#ifndef SURFACE_H
#define SURFACE_H

#include <QSqlQuery>
#include "settings.h"
#include "pointonray.h"
#include "eval_mpf.h"
#include "linalg.h"

class PointOnRay;

class Surface
{
    int id;                         // id in database
    int dimensionM;                 // I would like mkm (10e-6 m) (dimensionM == -6)
    QByteArray equation;            // initial string with equation of surface. ex "x*x + y*y - 1.0"
    QByteArray modifiedEquation;    // modified string with equation of surface. ex "(x*x + y*y - 1)/(x - 1.0)"
    QByteArray name;
    QByteArray description;
    LinAlg linAlg;                  // for NewtonMetodForSurfaceAndStraightLine(...)

public:
    Surface(int tId);
    ~Surface();
    QByteArray getEquation() const;
    const char *getEquationConstData() const;
    QByteArray getModifiedEquation() const;
    void setModifiedEquation(const QByteArray &value);

    //+- get intersecrion point of Surface(modifiedEquation) and StraightLine(prevP and dirVec)
    int NewtonMetodForSurfaceAndStraightLine(PointOnRay *prevPoint, // any point belonging to a straight line
                            PointOnRay *resultPoint, // here will be result - intersection point
                            const REAL *accuracy,
                            int maximumNumberOfAttempts = 500);
};

#endif // SURFACE_H
