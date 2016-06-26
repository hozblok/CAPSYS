#ifndef SETTINGS_H
#define SETTINGS_H

#define CAPSYS_VERSION 	2.0

#include <algorithm> //std::swap()
#include <math.h>
#include <time.h> //for random
#include <QtCore>

#define USE_MPF_CLASS 1

#ifndef M_PI
#define M_PI      3.14159265358979323846
#endif //M_PI

#ifndef M_PI_STR //512 bit:
#define M_PI_STR "3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481117"
#endif //M_PI_STR


#if USE_MPF_CLASS
#include <gmpxx.h>
#include <mpfr.h>
#include <mpf2mpfr.h>
#define prev_mpf_clear __gmpf_clear
typedef mpf_class REAL; // mpf_class !not mpfr_t;
const int NUMBITS = 512;
const REAL ZERO("0.0", NUMBITS);
const REAL ONE("1.0", NUMBITS);
const REAL MPF_PI(M_PI_STR, NUMBITS);
const REAL PlanckConstant("4.135667662e-15", NUMBITS); // eV / s
const REAL SpeedOfLight("299792458", NUMBITS); // m / s

class Utils
{
private:
    gmp_randclass random;
public:
    Utils();
    ~Utils();
    REAL getRandom(REAL &lower, REAL &upper);
    REAL getRandom();
};

static Utils utils;
#else
typedef double REAL;
#endif //USE_MPF_CLASS

typedef QVector< QVector< REAL > > VEC_VEC_R;
typedef QVector< size_t > VEC_I;
typedef QVector< REAL > VEC_R;

//+ three-dimensional space and the name of variables by default
const int sizeColumns = 3; // number of variable   == 3
const int sizeRows = 3; // number of equations, == 3
const char namesOfVariables[sizeColumns] = {'x','y','z'};
static QMap<char, int> create_index_for_variables()
{
    QMap<char, int> map;
    map['x'] =  0;
    map['y'] =  1;
    map['z'] =  2;

    return map;
}
const QMap<char, int> indexesOfVariables = create_index_for_variables();
//-
//+ simple output
int output(const char * message);
//-
//+ a readable string of mpf_class numbers:
QString getQString(const REAL value, bool flagE = false);
//-






#endif // SETTINGS_H
