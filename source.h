#ifndef SOURCE_H
#define SOURCE_H

#include "settings.h"
#include "eval_mpf.h"

class Source
{
private:
    const char * surface;
    //+ borders of source:
    REAL x_lower, x_upper;
    REAL y_lower, y_upper;
    REAL z_lower, z_upper;
    //-

    //+ borders for shooting
    REAL phi_lower, phi_upper;
    REAL theta_lower, theta_upper;
    //-

    REAL speed_of_light;

public:
    Source(const char * surface_source
           , const char * t_x_lower
           , const char * t_x_upper
           , const char * t_y_lower
           , const char * t_y_upper
           , const char * t_z_lower
           , const char * t_z_upper
           , const char * t_phi_lower
           , const char * t_phi_upper
           , const char * t_theta_lower
           , const char * t_theta_upper);
    ~Source();
    int get_random_x_y_z(VEC_R &point_source, int num_of_attempts = 200);
    int get_random_phi_theta(REAL &phi, REAL &theta);
    int get_random_phi_theta(VEC_R &directing_vec, REAL &phi, REAL &theta);

};

#endif // SOURCE_H
