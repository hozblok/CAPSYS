#include "source.h"

Source::Source(const char * surface_source
               , const char * t_x_lower
               , const char * t_x_upper
               , const char * t_y_lower
               , const char * t_y_upper
               , const char * t_z_lower
               , const char * t_z_upper
               , const char * t_phi_lower
               , const char * t_phi_upper
               , const char * t_theta_lower
               , const char * t_theta_upper) :
    surface(surface_source)
    , x_lower(t_x_lower, NUMBITS)
    , x_upper(t_x_upper, NUMBITS)
    , y_lower(t_y_lower, NUMBITS)
    , y_upper(t_y_upper, NUMBITS)
    , z_lower(t_z_lower, NUMBITS)
    , z_upper(t_z_upper, NUMBITS)
    , phi_lower(t_phi_lower, NUMBITS)
    , phi_upper(t_phi_upper, NUMBITS)
    , theta_lower(t_theta_lower, NUMBITS)
    , theta_upper(t_theta_upper, NUMBITS)
    , speed_of_light("0.0", NUMBITS)
{
    //?? speed_of_light = ...
}

Source::~Source()
{

}

int Source::get_random_x_y_z(VEC_R &point_source, int num_of_attempts)
{
    Eval eval(surface);
    REAL rand_x("0.0", NUMBITS), rand_y("0.0", NUMBITS), rand_z("0.0", NUMBITS);
    REAL tmp("0.0", NUMBITS);
    int i = 0;
    do
    {
        rand_x = utils.getRandom(x_lower, x_upper);
        rand_y = utils.getRandom(y_lower, y_upper);
        rand_z = utils.getRandom(z_lower, z_upper);
        tmp = eval.getValue(rand_x, rand_y, rand_z);
        if (i > num_of_attempts)
        {
            output("ERROR: get_random_x_y_z: exceeded the number of attempts!");
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
    phi = utils.getRandom(phi_lower, phi_upper);
    theta = utils.getRandom(theta_lower, theta_upper);
#ifdef DEBUG
    qDebug() << "DEBUG: Source::get_random_phi_theta: phi, theta:" << phi.get_d() << theta.get_d();
#endif
    return 0;
}

int Source::get_random_phi_theta(VEC_R &directing_vec, REAL &phi, REAL &theta)
{
    phi = utils.getRandom(phi_lower, phi_upper);
    theta = utils.getRandom(theta_lower, theta_upper);

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
