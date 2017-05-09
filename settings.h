#ifndef SETTINGS_H
#define SETTINGS_H

#include <math.h>
#include <time.h>       // for random
#include <algorithm>    // for std::swap()
#include <sstream>      // for IntToStr()
#include <vector>
#include <map>
#include <string>
#include <QTime>

#ifndef M_PI
#define M_PI            3.14159265358979323846
#endif
#ifndef M_PI_STR        // 512 bit:
#define M_PI_STR        "3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481117"
#endif
#define IntToStr( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

#define CAPSYS_VERSION 	2.0
#define USE_MPF_CLASS   1

#if USE_MPF_CLASS
#include <gmpxx.h>
#include <mpfr.h>
#include <mpf2mpfr.h>
#define prev_mpf_clear __gmpf_clear
typedef mpf_class REAL; // mpf_class from GNU MP library !not mpfr_t;
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
    REAL getRandom(const REAL * const lower, const REAL * const upper);
    REAL getRandom();
    int getSimpleRandomInt(int N); //optional (can be excluded)
};

static Utils utils;
#else
typedef double REAL;
#endif //USE_MPF_CLASS

typedef std::vector< std::vector< REAL > > VEC_VEC_R;
typedef std::vector< size_t > VEC_I;
typedef std::vector< REAL > VEC_R;

//+ three-dimensional space and the name of variables by default
const int sizeColumns = 3;  // number of variable   == 3
const int sizeRows = 3;     // number of equations  == 3
const char namesOfVar[sizeColumns] = {'x','y','z'};
static std::map<char, int> createIndexForVariables()
{
    std::map<char, int> map;
    map['x'] =  0;
    map['y'] =  1;
    map['z'] =  2;

    return map;
}
const std::map<char, int> indexesOfVar = createIndexForVariables();
//-

//+ a readable string of mpf_class numbers:
std::string getString(const REAL value, bool flagE = false);
//-

//+ simple output
#include <QDebug>
#include <QVariant>
void print(const char * message, bool information = false);
//-





#endif // SETTINGS_H
