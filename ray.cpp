#include "ray.h"

Ray::Ray(Source *tSource, int tId) :
    id(tId),
    source(tSource),
    capillary(0),
    xSource("0.0", NUMBITS),
    ySource("0.0", NUMBITS),
    zSource("0.0", NUMBITS),
    phi("0.0", NUMBITS),
    theta("0.0", NUMBITS),
    xDirectingVec("0.0", NUMBITS),
    yDirectingVec("0.0", NUMBITS),
    zDirectingVec("0.0", NUMBITS),
    captured(false),
    reachedScreen(false),
    currentPoint(0),
    nextPoint(0)
{
    setXYZSource();

    setPhiThetaDirVec();

    QSqlQuery query;
    query.prepare("INSERT INTO rays (sources_id, caps_id, phi, theta, x_source, y_source, z_source, captured, reached_screen)"
                                  "VALUES (:sources_id, :caps_id, :phi, :theta, :x_source, :y_source, :z_source, :captured, :reached_screen);");
    query.bindValue(":sources_id", source->getId());
//    query.bindValue(":caps_id", NULL);
    query.bindValue(":phi", QString::fromStdString(getString(phi)));
    query.bindValue(":theta", QString::fromStdString(getString(theta)));
    query.bindValue(":x_source", QString::fromStdString(getString(xSource)));
    query.bindValue(":y_source", QString::fromStdString(getString(ySource)));
    query.bindValue(":z_source", QString::fromStdString(getString(zSource)));
    query.bindValue(":captured", 0);
    query.bindValue(":reached_screen", 0);
    query.exec();
    id = query.lastInsertId().toInt();

    print((QString("#creating Ray, id:") + QString::number(id)).toLocal8Bit().constData(), true);

    createPoint();
}

int Ray::setXYZSource()
{
    Eval eval(source->getSurface()->getEquationConstData());
    REAL tmp("0.0", NUMBITS);
    int num_of_attempts = 100; //?? just in case
    int i = 0;
    do
    {
        xSource = utils.getRandom(source->getXLower(), source->getXUpper());
        ySource = utils.getRandom(source->getYLower(), source->getYUpper());
        zSource = utils.getRandom(source->getZLower(), source->getZUpper());
        tmp = eval.getValue(xSource, ySource, zSource);
        if (i > num_of_attempts)
        {
            print("ERROR: get_random_x_y_z: exceeded the number of attempts!");
            return -1;
        }
        ++i;
    }
    while (tmp > ZERO);

//    xSource = "0.00891820326340745159622503374887338147025450896904086674222003761061925047263313032264871449220233265885771603570865638400856061716313767513452591030634610981";
//    ySource = "0.0078728299136144123612453610130165624350980949446623566739663170757828953591874705668514898867820909729724928762649650379517412278455163911496661214314465272";
//    zSource = "-200.0";
//    xSource = "-0.000331348846935805927890718199785944531366459502311514973019759256905910129837636143352225258911995060931854186419196067848233744730989589572232435476697039158";
//    ySource = "-0.000252498239415041479673618494127089264744216377032178242707236069449477433517893647837611149896305672089949906744431932269219394540433496668098849977510462558";
//    zSource = "-700.0";

    return 0;
}

int Ray::setPhiThetaDirVec()
{

    phi = utils.getRandom(source->getPhiLower(), source->getPhiUpper());
    theta = utils.getRandom(source->getThetaLower(), source->getThetaUpper());

//    phi = "0.859400985359577983347432004881730086444736798219626924705158604377759334301092403221924517218627297236633973721598251998460597250942113004804319616643288901";
//    theta = "-0.0000193159077550345615090556838440020064769474059051326269506160830911371285946705276526889049736152677326126895783480338849908713386366846036831590862313127136";
//    phi = "0.0612055441907874802353341041082064251163426103164399756600411850084326663447239503392556415171354308363285413862620315755159693782884545786749208066833901331";
//    theta = "-0.000136736546572443604959991819120609931717171833044907580688630031767080823730990343420390422918173613733979537436750180833552984552744272595569616057505552406";

    QByteArray error;
    xDirectingVec = _cos(phi, ZERO, error) * _sin(theta, ZERO, error);  // std::cos(phi) * std::sin(theta);
    yDirectingVec = _sin(phi, ZERO, error) * _sin(theta, ZERO, error);  // std::sin(phi) * std::sin(theta);
    zDirectingVec = _cos(theta, ZERO, error);                           // std::cos(theta);

    if (!error.isEmpty())
    {
        return -1;
    }
    return 0;
}

int Ray::createPoint()
{
    currentPoint = new PointOnRay(this, true, false);      //first point of ray
    nextPoint = new PointOnRay(this, false, false); //now that's empty point
    return 0;
}

int Ray::recreateNextPoint()
{
    nextPoint->setModified(false);
    delete nextPoint;
    nextPoint = new PointOnRay(this, false, false);
    return 0;
}

int Ray::acceptNextPoint()
{
    delete currentPoint;
    currentPoint = nextPoint;
    nextPoint = new PointOnRay(this, false, false);
    return 0;
}

Ray::~Ray()
{
    delete currentPoint;
    currentPoint = 0;
    delete nextPoint;
    nextPoint = 0;
    delete capillary;
    capillary = 0;
    print((QString("#deleting Ray, id:") + QString::number(id)).toLocal8Bit().constData(), true);
}

int Ray::getId() const
{
    return id;
}

REAL Ray::getXSource() const
{
    return xSource;
}

REAL Ray::getYSource() const
{
    return ySource;
}

REAL Ray::getZSource() const
{
    return zSource;
}

REAL Ray::getXDirectingVec() const
{
    return xDirectingVec;
}

REAL Ray::getYDirectingVec() const
{
    return yDirectingVec;
}

REAL Ray::getZDirectingVec() const
{
    return zDirectingVec;
}

PointOnRay *Ray::getCurrentPoint() const
{
    return currentPoint;
}

PointOnRay *Ray::getNextPoint() const
{
    return nextPoint;
}

bool Ray::getCaptured() const
{
    return captured;
}

void Ray::setCaptured(bool value)
{
    captured = value;
}

Capillary *Ray::getCapillary() const
{
    return capillary;
}

void Ray::setCapillary(Capillary *value)
{
    capillary = value;
}
