#include "project.h"

Project::Project(int projId, QObject *parent) :
    QObject(parent),
    id(projId)
{
    print((QString("#creating Project, id:") + QString::number(id)).toLocal8Bit().constData(), true);

    // get capsIds
    QSqlQuery query;
    query.prepare("SELECT id, caps_id "
                  "FROM caps_projs "
                  "WHERE projs_id = ?");
    query.addBindValue(id);
    query.exec();
    while (query.next())
    {
        capsIds.push_back(query.value("caps_id").toInt());
    }
    // get screensIds
    query.prepare("SELECT screens_id "
                  "FROM screens_projs "
                  "WHERE projs_id = ?");
    query.addBindValue(id);
    query.exec();
    while (query.next())
    {
        screensIds.push_back(query.value("screens_id").toInt());
    }
    // get sourcesIds
    query.prepare("SELECT id, sources_id, number_of_beams "
                  "FROM sources_projs "
                  "WHERE projs_id = ?");
    query.addBindValue(id);
    query.exec();
    while (query.next())
    {
        sourcesIds.push_back(query.value("sources_id").toInt());
        if (query.value("number_of_beams").isNull() || query.value("number_of_beams").toInt() <= 0)
            numsOfBeams.push_back(1);
        else
            numsOfBeams.push_back(query.value("number_of_beams").toInt());
    }

    qDebug() << "capsIds: " << capsIds << screensIds << sourcesIds << numsOfBeams;
}

Project::~Project()
{
    print((QString("#deleting Project, id:") + QString::number(id)).toLocal8Bit().constData(), true);
}

int Project::startModelling()
{
    QVector<int>::const_iterator itSou = sourcesIds.cbegin();
    QVector<int>::const_iterator itNum = numsOfBeams.cbegin();
    // for each source
    for (; itSou < sourcesIds.cend(); ++itSou, ++itNum)
    {
        // for each ray
        for (int iRay = 0; iRay < *itNum; ++iRay)
        {
            // create source
            Source source(*itSou);
            // create ray for source
            Ray *ray = source.makeRay();
            // try to find capillary that capture source.ray (if such exist)
            Capillary *capillary = 0;
            QVector<int>::const_iterator it_cap = capsIds.cbegin();
            for (; it_cap < capsIds.cend(); ++it_cap)
            {
                // create capillary
                capillary = new Capillary(*it_cap);
                int tmpResult = capillary->getLowerEdge()->NewtonMetodForSurfaceAndStraightLine(
                            ray->getCurrentPoint(),
                            ray->getNextPoint(),
                            capillary->getAccuracy());
                if (tmpResult != 0)
                {
                    print("WARNING: beam could not reach the bottom of the capillary border!");
                    continue;
                }
                Eval evalSurfCap(capillary->getSurface()->getEquationConstData());
                if (evalSurfCap.getValue(*ray->getNextPoint()->getX(),
                                         *ray->getNextPoint()->getY(),
                                         *ray->getNextPoint()->getZ()) < ZERO)
                {
                    // capillary found
                    ray->setCaptured(true);
                    ray->setCapillary(capillary);
                    break;
                }
                else
                {
                    // capillary does not found
                    ray->setCaptured(false);
                    delete capillary;
                }
            }
            if (!ray->getCaptured()) // if capillary for ray does not exist
            {
                // write to db how it is.
                // точки, которые не захватились - не запишутся.
                // для этого просто переходим к следующему лучу
                print("@@@ OK: not_captured!");
                //?? убрать принт, проверить, что всё ок записалось!
                break;
            }
            // capillary was found
            // deal with ray futher

            // looking for a first point (and other) of intersection line with the capillary
            int number_of_attempts = 500; //?? должно быть записано где-то в бд???
            int number_point = 1;
            bool error = false;
            for (; ;++number_point)
            {
                if (number_point > number_of_attempts * 10)
                {
                    print("ERROR: Send ray: number_point > number_of_attempts");
                    error = true;
                    break;
                    //?? записывать в бд, что не удалось обсчитать луч???
                }
                int tmp_result = capillary->getSurface()->NewtonMetodForSurfaceAndStraightLine(
                                ray->getCurrentPoint(),
                                ray->getNextPoint(),
                                capillary->getAccuracy());
                if (tmp_result != 0)
                {
                    error = true;
                    break;
                    //?? записывать в бд, что не удалось обсчитать луч???
                }
                //+ if the beam flew in the wrong direction:
                // eval_lower.getValue(tmp_result_point[0], tmp_result_point[1], tmp_result_point[2]) < ZERO --- for ane point and for first
                // or tmp_wrong += (tmp_result_point[j] - prev_point[j]) * directing_vector[j]; tmp_wrong < ZERO
                Eval eval_lower(capillary->getLowerEdge()->getEquationConstData());
                REAL tmp_wrong("0.0", NUMBITS);
                tmp_wrong += (*ray->getNextPoint()->getX() - *ray->getCurrentPoint()->getX()) * *ray->getCurrentPoint()->getXDirectingVec()
                           + (*ray->getNextPoint()->getY() - *ray->getCurrentPoint()->getY()) * *ray->getCurrentPoint()->getYDirectingVec()
                           + (*ray->getNextPoint()->getZ() - *ray->getCurrentPoint()->getZ()) * *ray->getCurrentPoint()->getZDirectingVec();
                qDebug() << "003";
                while (tmp_wrong < ZERO
                       || eval_lower.getValue(*ray->getCurrentPoint()->getX(), *ray->getCurrentPoint()->getY(), *ray->getCurrentPoint()->getZ()) < ZERO)
                {
                    qDebug() << "004";
                    // construct surface_of_cap_modified
                    if (*ray->getNextPoint()->getZ() >= ZERO)
                    {
                        QByteArray surface_of_cap_modified = "(" + capillary->getSurface()->getModifiedEquation() + ")/(z-" + getString(*ray->getNextPoint()->getZ()).data() + ")";
                        capillary->getSurface()->setModifiedEquation(surface_of_cap_modified);
                    }
                    else
                    {
                        QByteArray surface_of_cap_modified = "(" + capillary->getSurface()->getModifiedEquation() + ")/(z+" + getString(abs(*ray->getNextPoint()->getZ())).data() + ")";
                        capillary->getSurface()->setModifiedEquation(surface_of_cap_modified);
                    }
                    ray->recreateNextPoint();
                    tmp_result = capillary->getSurface()->NewtonMetodForSurfaceAndStraightLine(
                                ray->getCurrentPoint(),
                                ray->getNextPoint(),
                                capillary->getAccuracy());
                    if (tmp_result != 0)
                    {
                        qDebug() << "ERROR: Send Ray 1.3: continue";
                        error = true;
                        break;
                        //?? записывать в бд, что не удалось обсчитать луч???
                    }
                    tmp_wrong = ZERO;
                    tmp_wrong += (*ray->getNextPoint()->getX() - *ray->getCurrentPoint()->getX()) * *ray->getCurrentPoint()->getXDirectingVec()
                            + (*ray->getNextPoint()->getY() - *ray->getCurrentPoint()->getY()) * *ray->getCurrentPoint()->getYDirectingVec()
                            + (*ray->getNextPoint()->getZ() - *ray->getCurrentPoint()->getZ()) * *ray->getCurrentPoint()->getZDirectingVec();
                }
                if (error)
                {
                    break;
                }
                //- if the beam flew in the wrong direction

                //+ if the beam back to the same point
                for (int j = 0;
                     abs(*ray->getNextPoint()->getX() - *ray->getCurrentPoint()->getX()) <= *capillary->getAccuracyPhysics()
                     && abs(*ray->getNextPoint()->getY() - *ray->getCurrentPoint()->getY()) <= *capillary->getAccuracyPhysics()
                     && abs(*ray->getNextPoint()->getZ() - *ray->getCurrentPoint()->getZ()) <= *capillary->getAccuracyPhysics(); ++j)
                {
                    if (j > number_of_attempts)
                    {
                        print("ERROR: Send Ray: Failed to find the second point of intersection of the beam. capillary has a hole?");
                        error = true;
                        break;
                        //?? запись в бд делаем?
                    }
                    ray->recreateNextPoint();
                    tmp_result = capillary->getSurface()->NewtonMetodForSurfaceAndStraightLine(
                                ray->getCurrentPoint(),
                                ray->getNextPoint(),
                                capillary->getAccuracy());
                    if (tmp_result != 0)
                    {
                        //??
                        print("ERROR: Send Ray 2.2: continue");
                        continue;
                    }
                }
                if (error)
                {
                    break;
                }
                //- if the beam back to the same point

                //ok, the next point was got succesfully.



                //+ paranoid_check
                //??
                bool paranoid_check = false;
//                qDebug() << "!paranoid_check: " << paranoid_check;
                if (paranoid_check)
                {
//                    //+теперь надо проверить, что между prev_point и tmp_result_point нет ещё одной точки - это очень важная проверка
//                    //будем искать до тех пор, пока не сломается NewtonMetodForSurfaceAndStraightLine
//                    bool continue_checking = true;
//                    REAL tmpVecA("0.0", NUMBITS);
//                    REAL tmpVecB("0.0", NUMBITS);
//                    PointOnRay *tmpPointForCheck = new PointOnRay(ray, false, false);
//                    tmpPointForCheck->setX(*ray->getNextPoint()->getX());
//                    tmpPointForCheck->setY(*ray->getNextPoint()->getY());
//                    tmpPointForCheck->setZ(*ray->getNextPoint()->getZ());
//                    while (continue_checking)
//                    {
//                        if(tmpVecA > ZERO && tmpVecB < ZERO)
//                        {
//                            qDebug() << "НИЧЕГО СЕБЕ!!! эта точка находится между prev_point и tmp_result_point";
//                            ray->recreateNextPoint(tmpPointForCheck);

//                            surface_of_cap_modified = "(" + surface_of_cap_modified + ")/(z-(" + getString(tmp_result_point[2]).data() + "))";
//                        }
//                        else
//                        {
//                            surface_of_cap_modified = "(" + surface_of_cap_modified + ")/(z-(" + getString(tmp_result_point_check[2]).data() + "))";
//                        }
//                        tmp_result = capillary->getSurface()->NewtonMetodForSurfaceAndStraightLine(
//                                    ray->getCurrentPoint(),
//                                    ray->getNextPoint(),
//                                    )
//                        tmp_result = linalg.NewtonMetodForSurfaceAndStraightLine(surface_of_cap
//                                                                                 , surface_of_cap_modified
//                                                                                 , prev_point
//                                                                                 , tmp_result_point_check
//                                                                                 , directing_vector
//                                                                                 , accuracy);
//                        if (tmp_result != 0)
//                        {
//                            continue_checking = false;
//                            // tmp_result_point is truth point!
//                            break;
//                        }
//                        for (int j = 0; j < sizeColumns; ++j)
//                        {
//                            tmpVecA += (tmp_result_point_check[j] - prev_point[j]) * directing_vector[j];
//                            tmpVecB += (tmp_result_point_check[j] - tmp_result_point[j]) * directing_vector[j];
//                        }
//                        qDebug() << "@@@&&&@@@";
//                    }
                }
                //- paranoid_check

                qDebug() << "010";

                // Point was found, now check that the beam hit the screen.
                // Set a new approach and new direction vector!
                Eval eval_upper(capillary->getUpperEdge()->getEquationConstData());
                if (eval_upper.getValue(*ray->getNextPoint()->getX(),
                                        *ray->getNextPoint()->getY(),
                                        *ray->getNextPoint()->getZ()) > ZERO) //the ray has gone to the screen
                {
                    ray->getNextPoint()->setOnScreen(true);
                    break;
                }
                qDebug() << "011_";
                if (eval_lower.getValue(*ray->getNextPoint()->getX(),
                                        *ray->getNextPoint()->getY(),
                                        *ray->getNextPoint()->getZ()) < ZERO) //the ray turn round to the screen
                {
                    //?? что делать мы же проверили, что луч правильно пошёл?
                    error = true;
                    bool flag_ray_departed = true;
                    break;
                }

                qDebug() << "012";
                //+- ok, now need to turn directing vector
                VEC_R tmp_normal_to_surface(sizeRows);
                for (VEC_R::iterator it = tmp_normal_to_surface.begin(); it != tmp_normal_to_surface.end(); ++it)
                {
                    it->set_prec(NUMBITS);
                }
                Eval evalSurfCap(capillary->getSurface()->getEquationConstData());
                for (int j = 0; j < sizeColumns; ++j)
                {
                    tmp_normal_to_surface[j] = evalSurfCap.getValueDerivative(namesOfVar[j],
                                                                               *ray->getNextPoint()->getX(),
                                                                               *ray->getNextPoint()->getY(),
                                                                               *ray->getNextPoint()->getZ());
                }
                REAL tmp("0.0", NUMBITS), tmp_projection("0.0", NUMBITS);
                tmp = sqrt(tmp_normal_to_surface[0] * tmp_normal_to_surface[0] +
                           tmp_normal_to_surface[1] * tmp_normal_to_surface[1] +
                           tmp_normal_to_surface[2] * tmp_normal_to_surface[2]);
                for (int j = 0; j < sizeColumns; ++j)
                {
                    tmp_normal_to_surface[j] /= tmp;  // нормированный вектор внутренней нормали
                }

                qDebug() << "013";
                tmp_projection = tmp_normal_to_surface[0] * *ray->getCurrentPoint()->getXDirectingVec()
                               + tmp_normal_to_surface[1] * *ray->getCurrentPoint()->getYDirectingVec()
                               + tmp_normal_to_surface[2] * *ray->getCurrentPoint()->getZDirectingVec(); // проекция прошлого направл. вектора на нормаль
                tmp = sqrt(*ray->getCurrentPoint()->getXDirectingVec() * *ray->getCurrentPoint()->getXDirectingVec() +
                           *ray->getCurrentPoint()->getYDirectingVec() * *ray->getCurrentPoint()->getYDirectingVec() +
                           *ray->getCurrentPoint()->getZDirectingVec() * *ray->getCurrentPoint()->getZDirectingVec());
                // calculate glancing angle
                QByteArray error_;
                REAL angle_("0.0", NUMBITS);
                angle_ = _asin(tmp_projection / tmp, ZERO, error_); //angle = asin(tmp_projection / tmp);
                ray->getNextPoint()->setAngle(angle_);
                if (angle_ < ZERO)
                {
                    error = true;
                    qDebug() << "### ERROR: ANGLEGLACING < 0!";
                    break;
                }

                qDebug() << "014";
                tmp = *ray->getCurrentPoint()->getXDirectingVec() - 2.0 * abs(tmp_projection) * tmp_normal_to_surface[0]; //новый направляющий вектор прямой)
                ray->getNextPoint()->setXDirectingVec(tmp);
                tmp = *ray->getCurrentPoint()->getYDirectingVec() - 2.0 * abs(tmp_projection) * tmp_normal_to_surface[1]; //новые = для новой прямой
                ray->getNextPoint()->setYDirectingVec(tmp);
                tmp = *ray->getCurrentPoint()->getZDirectingVec() - 2.0 * abs(tmp_projection) * tmp_normal_to_surface[2]; //для старой прямой значение потеряно и не нужно
                ray->getNextPoint()->setZDirectingVec(tmp);

                qDebug() << "015";
                //+ new current and next point
                if (ray->acceptNextPoint() != 0)
                {
                    error = true;
                    break;
                }
                //-
                //+ new initial approximation - not necessary!
                //for (int j = 0; j < sizeColumns; ++j)
                //{
                //    tmp_result_point[j] += directing_vector[j]; //not necessary
                //}
                //-

                qDebug() << "016";
                //+ return the surface equation back
                capillary->getSurface()->setModifiedEquation(capillary->getSurface()->getEquation());
                //-
            }
            //??
            break; //DEBUG - one ray!

        } // for each ray

    } // for each source
    return 0;
}
