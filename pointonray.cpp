#include "pointonray.h"


PointOnRay::PointOnRay(Ray *tRay, bool tOnSource, bool tOnScreen, Screen *tScreen):
    id(0),
    ray(tRay),
    numberInRay(0),
    x("0.0", NUMBITS),
    y("0.0", NUMBITS),
    z("0.0", NUMBITS),
    xDirectingVec("0.0", NUMBITS),
    yDirectingVec("0.0", NUMBITS),
    zDirectingVec("0.0", NUMBITS),
    angle("0.0", NUMBITS),
    onSource(tOnSource),
    onScreen(tOnScreen),
    screen(tScreen),
    modified(false)
{
    if (onSource)
    {
        numberInRay = 0;
        x = ray->getXSource();
        y = ray->getYSource();
        z = ray->getZSource();
        xDirectingVec = ray->getXDirectingVec();
        yDirectingVec = ray->getYDirectingVec();
        zDirectingVec = ray->getZDirectingVec();

        //++++++++++++++++POINT+DB+++++++++++++++++++++++
        QSqlQuery query;
        query.prepare("INSERT INTO points "
                      "(rays_id, number_in_ray, "
                      "x, y, z, "
                      "x_dir_vec, y_dir_vec, z_dir_vec, "
                      "angle, on_source, on_screen)"
                      "VALUES (:rays_id, :number_in_ray, "
                      ":x, :y, :z, "
                      ":x_dir_vec, :y_dir_vec, :z_dir_vec, "
                      ":angle, :on_source, :on_screen);");
        query.bindValue(":rays_id", ray->getId());
        query.bindValue(":number_in_ray", numberInRay);
        query.bindValue(":x", QString::fromStdString(getString(x)));
        query.bindValue(":y", QString::fromStdString(getString(y)));
        query.bindValue(":z", QString::fromStdString(getString(z)));
        query.bindValue(":x_dir_vec", QString::fromStdString(getString(xDirectingVec)));
        query.bindValue(":y_dir_vec", QString::fromStdString(getString(yDirectingVec)));
        query.bindValue(":z_dir_vec", QString::fromStdString(getString(zDirectingVec)));
        query.bindValue(":angle", QString::fromStdString(getString(angle))); //"0."
        query.bindValue(":on_source", onSource);
        query.bindValue(":on_screen", onScreen);
        query.exec();
        id = query.lastInsertId().toInt();
        //---------------------------------------------
        print((QString("#creating PointOnRay, id:") + QString::number(id)).toLocal8Bit().constData(), true);
    }
    else if (onScreen)
    {
        //??
    }
    else
    {
        numberInRay = ray->getCurrentPoint()->getNumberInRay() + 1;
        //+ get random aproximation   optional (can be excluded)
        // 10e13
        QByteArray randRange = "10e" + QString::number(utils.getSimpleRandomInt(12)).toLocal8Bit();
        REAL randNum(randRange.constData(), NUMBITS);
        REAL randNumLower(randRange.prepend("-").constData(), NUMBITS);
        randNum = utils.getRandom(randNumLower, randNum);
        x = *ray->getCurrentPoint()->getX() + randNum * *ray->getCurrentPoint()->getXDirectingVec();
        y = *ray->getCurrentPoint()->getY() + randNum * *ray->getCurrentPoint()->getYDirectingVec();
        z = *ray->getCurrentPoint()->getZ() + randNum * *ray->getCurrentPoint()->getZDirectingVec();
        //-
        print((QString("#creating PointOnRay, id:") + QString::number(id)).toLocal8Bit().constData(), true);
    }
}

PointOnRay::~PointOnRay()
{
    print((QString("#deleting PointOnRay, id:") + QString::number(id)).toLocal8Bit().constData(), true);
    if (modified && numberInRay > 0 && !onSource && ray->getCaptured())
    {
        QSqlQuery query;
        query.prepare("INSERT INTO points "
                      "(rays_id, number_in_ray, "
                      "x, y, z, "
                      "x_dir_vec, y_dir_vec, z_dir_vec, "
                      "angle, on_source, on_screen, screens_id)"
                      "VALUES (:rays_id, :number_in_ray, "
                      ":x, :y, :z, "
                      ":x_dir_vec, :y_dir_vec, :z_dir_vec, "
                      ":angle, :on_source, :on_screen, :screens_id);");
        query.bindValue(":rays_id", ray->getId());
        query.bindValue(":number_in_ray", numberInRay);
        query.bindValue(":x", QString::fromStdString(getString(x)));
        query.bindValue(":y", QString::fromStdString(getString(y)));
        query.bindValue(":z", QString::fromStdString(getString(z)));
        query.bindValue(":x_dir_vec", QString::fromStdString(getString(xDirectingVec)));
        query.bindValue(":y_dir_vec", QString::fromStdString(getString(yDirectingVec)));
        query.bindValue(":z_dir_vec", QString::fromStdString(getString(zDirectingVec)));
        query.bindValue(":angle", QString::fromStdString(getString(angle)));
        query.bindValue(":on_source", onSource);
        query.bindValue(":on_screen", onScreen);
        if (onScreen)
        {
            query.bindValue(":screens_id", screen->getId());
        }
        query.exec();
        id = query.lastInsertId().toInt();
    }
    if (modified && numberInRay == 0 && onSource)
    {
        QSqlQuery query;
        query.prepare("UPDATE points "
                      "SET "
                      "rays_id = :rays_id, number_in_ray = :number_in_ray, "
                      "x = :x, y = :y, z = :z, "
                      "x_dir_vec = :x_dir_vec, y_dir_vec = :y_dir_vec, z_dir_vec = :z_dir_vec, "
                      "angle = :angle, on_source = :on_source, on_screen = :on_screen)"
                      "WHERE ID = :id ");
        query.bindValue(":rays_id", ray->getId());
        query.bindValue(":number_in_ray", numberInRay);
        query.bindValue(":x", QString::fromStdString(getString(x)));
        query.bindValue(":y", QString::fromStdString(getString(y)));
        query.bindValue(":z", QString::fromStdString(getString(z)));
        query.bindValue(":x_dir_vec", QString::fromStdString(getString(xDirectingVec)));
        query.bindValue(":y_dir_vec", QString::fromStdString(getString(yDirectingVec)));
        query.bindValue(":z_dir_vec", QString::fromStdString(getString(zDirectingVec)));
        query.bindValue(":angle", QString::fromStdString(getString(angle)));
        query.bindValue(":on_source", onSource);
        query.bindValue(":on_screen", onScreen);
        //screens_id - null
        query.bindValue(":id", id);
        query.exec();
    }
}


const REAL *PointOnRay::getX() const
{
    return &x;
}

void PointOnRay::setX(const REAL &value)
{
    x = value;
    modified = true;
}

const REAL *PointOnRay::getY() const
{
    return &y;
}

void PointOnRay::setY(const REAL &value)
{
    y = value;
    modified = true;
}

const REAL *PointOnRay::getZ() const
{
    return &z;
}

void PointOnRay::setZ(const REAL &value)
{
    z = value;
    modified = true;
}

const REAL *PointOnRay::getXDirectingVec() const
{
    return &xDirectingVec;
}

void PointOnRay::setXDirectingVec(const REAL &value)
{
    xDirectingVec = value;
    modified = true;
}

const REAL *PointOnRay::getYDirectingVec() const
{
    return &yDirectingVec;
}

void PointOnRay::setYDirectingVec(const REAL &value)
{
    yDirectingVec = value;
    modified = true;
}

const REAL *PointOnRay::getZDirectingVec() const
{
    return &zDirectingVec;
}

void PointOnRay::setZDirectingVec(const REAL &value)
{
    zDirectingVec = value;
    modified = true;
}

const REAL *PointOnRay::getAngle() const
{
    return &angle;
}

void PointOnRay::setAngle(const REAL &value)
{
    angle = value;
    modified = true;
}

int PointOnRay::getNumberInRay() const
{
    return numberInRay;
}

bool PointOnRay::getModified() const
{
    return modified;
}

void PointOnRay::setModified(bool value)
{
    modified = value;
}

bool PointOnRay::getOnScreen() const
{
    return onScreen;
}

void PointOnRay::setOnScreen(bool value)
{
    onScreen = value;
}


