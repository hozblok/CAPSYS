#ifndef SOURCE_H
#define SOURCE_H

#include <QSqlQuery>
#include "settings.h"
#include "surface.h"
#include "ray.h"
#include "eval_mpf.h"

class Surface;
class Ray;

class Source
{
    int id;
    int dimensionM; // I would like mkm (10e-6 m)
    Surface *surface;
    QByteArray surfaceSource;
    //+ borders of source:
    REAL xLower, xUpper;
    REAL yLower, yUpper;
    REAL zLower, zUpper;
    //-
    //+ borders for shooting:
    REAL phiLower, phiUpper;
    REAL thetaLower, thetaUpper;
    //-
    REAL speedOfLight;
    Ray *currentRay;
    QByteArray comment;

public:
    Source(const char *surface_source
           , const char *tXLower
           , const char *tXUpper
           , const char *tYLower
           , const char *tYUpper
           , const char *tZLower
           , const char *tZUpper
           , const char *tPhiLower
           , const char *tPhiUpper
           , const char *tThetaLower
           , const char *tThetaUpper);
    Source(int sourceId);
    ~Source();
    Ray *makeRay();
    int get_random_x_y_z(VEC_R &point_source, int num_of_attempts = 200);
    int get_random_phi_theta(REAL &phi, REAL &theta);
    int get_random_phi_theta(VEC_R &directing_vec, REAL &phi, REAL &theta);

    int getId() const {return id;}
    const Surface *getSurface() const {return surface;}

    const REAL *getXLower();
    const REAL *getXUpper();
    const REAL *getYLower();
    const REAL *getYUpper();
    const REAL *getZLower();
    const REAL *getZUpper();
    const REAL *getPhiLower();
    const REAL *getPhiUpper();
    const REAL *getThetaLower();
    const REAL *getThetaUpper();

};

#endif // SOURCE_H
