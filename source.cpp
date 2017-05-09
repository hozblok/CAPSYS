#include "source.h"


Source::Source(const char * surfaceSource
               , const char * tXLower
               , const char * tXUpper
               , const char * tYLower
               , const char * tYUpper
               , const char * tZLower
               , const char * tZUpper
               , const char * tPhiLower
               , const char * tPhiUpper
               , const char * tThetaLower
               , const char * tThetaUpper) :
    surface(0),
    surfaceSource(surfaceSource),
    xLower(tXLower, NUMBITS)
    , xUpper(tXUpper, NUMBITS)
    , yLower(tYLower, NUMBITS)
    , yUpper(tYUpper, NUMBITS)
    , zLower(tZLower, NUMBITS)
    , zUpper(tZUpper, NUMBITS)
    , phiLower(tPhiLower, NUMBITS)
    , phiUpper(tPhiUpper, NUMBITS)
    , thetaLower(tThetaLower, NUMBITS)
    , thetaUpper(tThetaUpper, NUMBITS)
    , speedOfLight("0.0", NUMBITS)
    , currentRay(0)
{
    //?? speed_of_light = ...
}

Source::Source(int sourceId):
    id(sourceId),
    dimensionM(-6),
    surface(0),
    xLower("0.0", NUMBITS),
    xUpper("0.0", NUMBITS),
    yLower("0.0", NUMBITS),
    yUpper("0.0", NUMBITS),
    zLower("0.0", NUMBITS),
    zUpper("0.0", NUMBITS),
    phiLower("0.0", NUMBITS),
    phiUpper("0.0", NUMBITS),
    thetaLower("0.0", NUMBITS),
    thetaUpper("0.0", NUMBITS),
    speedOfLight("0.0", NUMBITS),
    currentRay(0),
    comment("")
{
    print((QString("#creating Source, id:") + QString::number(id)).toLocal8Bit().constData(), true);

    // get source
    QSqlQuery query;
    query.prepare("SELECT surface_id, "
                  "x_lower, x_upper, "
                  "y_lower, y_upper, "
                  "z_lower, z_upper, "
                  "phi_lower, phi_upper, "
                  "theta_lower, theta_upper, "
                  "dimension_m, comment "
                  "FROM sources WHERE id = ?");
    query.addBindValue(id);
    query.exec();
    if (query.next())
    {
        surface     = new Surface(query.value("surface_id").toInt());
        xLower      = query.value("x_lower").toByteArray().constData();
        xUpper      = query.value("x_upper").toByteArray().constData();
        yLower      = query.value("y_lower").toByteArray().constData();
        yUpper      = query.value("y_upper").toByteArray().constData();
        zLower      = query.value("z_lower").toByteArray().constData();
        zUpper      = query.value("z_upper").toByteArray().constData();
        phiLower    = query.value("phi_lower").toByteArray().constData();
        phiUpper    = query.value("phi_upper").toByteArray().constData();
        thetaLower  = query.value("theta_lower").toByteArray().constData();
        thetaUpper  = query.value("theta_upper").toByteArray().constData();
        dimensionM  = query.value("dimension_m").toInt();
        comment     = query.value("comment").toByteArray();
    }
    else
    {
        //?? вызвать исключение!
    }
}

Source::~Source()
{
    delete currentRay;
    currentRay = 0;
    delete surface;
    surface = 0;
    print((QString("#deleting Source, id:") + QString::number(id)).toLocal8Bit().constData(), true);
}

Ray *Source::makeRay()
{
    currentRay = new Ray(this);
    return currentRay;
}

int Source::get_random_x_y_z(VEC_R &point_source, int num_of_attempts)
{
    Eval eval(surface->getEquationConstData());
    REAL rand_x("0.0", NUMBITS), rand_y("0.0", NUMBITS), rand_z("0.0", NUMBITS);
    REAL tmp("0.0", NUMBITS);
    int i = 0;
    do
    {
        rand_x = utils.getRandom(xLower, xUpper);
        rand_y = utils.getRandom(yLower, yUpper);
        rand_z = utils.getRandom(zLower, zUpper);
        tmp = eval.getValue(rand_x, rand_y, rand_z);
        if (i > num_of_attempts)
        {
            print("ERROR: get_random_x_y_z: exceeded the number of attempts!");
            return -1;
        }
        ++i;
    }
    while (tmp > ZERO);

//    rand_x = "0.00891820326340745159622503374887338147025450896904086674222003761061925047263313032264871449220233265885771603570865638400856061716313767513452591030634610981";
//    rand_y = "0.0078728299136144123612453610130165624350980949446623566739663170757828953591874705668514898867820909729724928762649650379517412278455163911496661214314465272";
//    rand_z = "-200.0";
//    rand_x = "-0.000331348846935805927890718199785944531366459502311514973019759256905910129837636143352225258911995060931854186419196067848233744730989589572232435476697039158";
//    rand_y = "-0.000252498239415041479673618494127089264744216377032178242707236069449477433517893647837611149896305672089949906744431932269219394540433496668098849977510462558";
//    rand_z = "-700.0";

    point_source[0] = rand_x;
    point_source[1] = rand_y;
    point_source[2] = rand_z;
#ifdef DEBUG
    qDebug() << "DEBUG: Source::get_random_x_y_z: point_source"<< point_source[0].get_d() << point_source[1].get_d() << point_source[2].get_d();
#endif
    return 0;
}

int Source::get_random_phi_theta(REAL &phi, REAL &theta)
{
    phi = utils.getRandom(phiLower, phiUpper);
    theta = utils.getRandom(thetaLower, thetaUpper);
#ifdef DEBUG
    qDebug() << "DEBUG: Source::get_random_phi_theta: phi, theta:" << phi.get_d() << theta.get_d();
#endif
    return 0;
}

int Source::get_random_phi_theta(VEC_R &directing_vec, REAL &phi, REAL &theta)
{
    phi = utils.getRandom(phiLower, phiUpper);
    theta = utils.getRandom(thetaLower, thetaUpper);

//    phi = "0.859400985359577983347432004881730086444736798219626924705158604377759334301092403221924517218627297236633973721598251998460597250942113004804319616643288901";
//    theta = "-0.0000193159077550345615090556838440020064769474059051326269506160830911371285946705276526889049736152677326126895783480338849908713386366846036831590862313127136";
//    phi = "0.0612055441907874802353341041082064251163426103164399756600411850084326663447239503392556415171354308363285413862620315755159693782884545786749208066833901331";
//    theta = "-0.000136736546572443604959991819120609931717171833044907580688630031767080823730990343420390422918173613733979537436750180833552984552744272595569616057505552406";

    QByteArray error;
    directing_vec[0] = _cos(phi, ZERO, error) * _sin(theta, ZERO, error); // std::cos(phi) * std::sin(theta);
    directing_vec[1] = _sin(phi, ZERO, error) * _sin(theta, ZERO, error); // std::sin(phi) * std::sin(theta);
    directing_vec[2] = _cos(theta, ZERO, error); // std::cos(theta);

#ifdef DEBUG
    //    qDebug() << phi.get_d() << theta.get_d();
    qDebug() << "DEBUG: Source::get_random_phi_theta: directing_vec" << directing_vec[0].get_d() << directing_vec[1].get_d() << directing_vec[2].get_d();
#endif
    return 0;
}

const REAL * Source::getXLower()
{
    return &xLower;
}

const REAL * Source::getXUpper()
{
    return &xUpper;
}

const REAL * Source::getYLower()
{
    return &yLower;
}

const REAL * Source::getYUpper()
{
    return &yUpper;
}

const REAL * Source::getZLower()
{
    return &zLower;
}

const REAL * Source::getZUpper()
{
    return &zUpper;
}

const REAL * Source::getPhiLower()
{
    return &phiLower;
}

const REAL * Source::getPhiUpper()
{
    return &phiUpper;
}


const REAL * Source::getThetaLower()
{
    return &thetaLower;
}


const REAL * Source::getThetaUpper()
{
    return &thetaUpper;
}
