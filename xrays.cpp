#include "xrays.h"

XRays::XRays(QObject *parent, QString connectionName) :
    QObject(parent)
{
    dbase = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    dbase.setDatabaseName("capsys.sqlite");
    if (!dbase.open()) {
        QString tmp = (QString) "ERROR: open_database: Unable to connect to database!" + dbase.lastError().text();
        print(tmp.toUtf8().data());
    }
}

bool XRays::open_database()
{
    if(!dbase.isOpen())
    {
        if (!dbase.open()) {
            QString tmp = (QString) "ERROR: open_database: Unable to connect to database!" + dbase.lastError().text();
            print(tmp.toUtf8().data());
            return false;
        }
    }
    return true;
}

int XRays::send_ray(VEC_I &cap_id, VEC_I &source_id, bool recording_not_captured, int number_of_attempts, bool paranoid_check)
{
    if(!open_database())
    {
        return -1;
    }
//    qDebug() << "DEBUG: cap_id" << cap_id[0] << "source_id" << source_id[0];

    //+ get cap from bd
    //+ получаем информацию о капилляре
    QSqlQuery query_cap(dbase);
    query_cap.prepare("SELECT id, surface, dimension_m, accuracy, accuracy_physics, lower_edge, upper_edge FROM caps WHERE id = ?");
    //??
    query_cap.addBindValue(cap_id[0]);
    query_cap.exec();
    QByteArray accur;
    QByteArray accur_phys;
    QByteArray surface_of_cap;
    QByteArray surface_of_cap_modified;
    QByteArray surface_lower_edge_of_cap;
    QByteArray surface_upper_edge_of_cap;
    if (query_cap.next())
    {
        accur = query_cap.value("accuracy").toByteArray();
        accur_phys = query_cap.value("accuracy_physics").toByteArray();
        surface_of_cap = query_cap.value("surface").toByteArray();
        surface_of_cap_modified = surface_of_cap;
        surface_lower_edge_of_cap = query_cap.value("lower_edge").toByteArray();
        surface_upper_edge_of_cap = query_cap.value("upper_edge").toByteArray();
#ifdef DEBUG
        qDebug() << "!!" <<query_cap.value("id").toInt() << accur << surface_of_cap << surface_lower_edge_of_cap << surface_upper_edge_of_cap;
#endif
    }
    else
    {
        print("ERROR: send_ray: caps no found!");
        return -1;
    }
    //-

    //+ get source from bd
    //+ получаем информацию об источнике
    QSqlQuery query_source(dbase);
    query_source.prepare("SELECT id, surface, x_lower, x_upper, y_lower, y_upper, z_lower, z_upper, phi_lower, phi_upper, theta_lower, theta_upper FROM sources WHERE id = ?");
    query_source.addBindValue(source_id[0]);
    query_source.exec();
    QByteArray surface_source;
    QByteArray x_lower, x_upper, y_lower, y_upper, z_lower, z_upper, phi_lower, phi_upper, theta_lower, theta_upper;
    if (query_source.next())
    {
        surface_source = query_source.value("surface").toByteArray();
        x_lower = query_source.value("x_lower").toByteArray();
        x_upper = query_source.value("x_upper").toByteArray();
        y_lower = query_source.value("y_lower").toByteArray();
        y_upper = query_source.value("y_upper").toByteArray();
        z_lower = query_source.value("z_lower").toByteArray();
        z_upper = query_source.value("z_upper").toByteArray();
        phi_lower = query_source.value("phi_lower").toByteArray();
        phi_upper = query_source.value("phi_upper").toByteArray();
        theta_lower = query_source.value("theta_lower").toByteArray();
        theta_upper = query_source.value("theta_upper").toByteArray();
#ifdef DEBUG
        qDebug() << "!!" << query_source.value("id").toInt() << surface_source << x_lower << x_upper << y_lower << z_lower << z_upper << phi_lower << phi_upper << theta_lower << theta_upper;
#endif
    }
    else
    {
        print("ERROR: send_ray: source no found!");
        return -1;
    }
    //-

    //+ initialization data
    REAL accuracy(accur.constData(), NUMBITS);
    REAL accuracy_phys(accur_phys.constData(), NUMBITS);
    REAL phi("0.0", NUMBITS);
    REAL theta("0.0", NUMBITS);
    REAL angle("0.0", NUMBITS); // glancing angle in each point
    //qDebug()() << "surface_of_cap" << surface_of_cap;
    Eval eval_surface(surface_of_cap);
    Eval eval_lower(surface_lower_edge_of_cap);
    Eval eval_upper(surface_upper_edge_of_cap);
    REAL z_upper_cap("0.0", NUMBITS);
    z_upper_cap = eval_upper.getValue(ZERO, ZERO, ZERO);
    Eval eval_surface_source(surface_source);
    Source source(surface_source, x_lower, x_upper
                  , y_lower, y_upper, z_lower, z_upper
                  , phi_lower, phi_upper, theta_lower, theta_upper);
    VEC_R point_source(sizeColumns);
    VEC_R directing_vector(sizeColumns);
    VEC_R prev_point(sizeColumns);
    VEC_R tmp_result_point(sizeColumns); VEC_R tmp_result_point_check(sizeColumns);
    VEC_R tmp_normal_to_surface(sizeColumns);
    for (int i = 0; i < sizeColumns; ++i)
    {
        point_source[i].set_prec(NUMBITS);
        directing_vector[i].set_prec(NUMBITS);
        prev_point[i].set_prec(NUMBITS);
        tmp_result_point[i].set_prec(NUMBITS);
        tmp_result_point_check[i].set_prec(NUMBITS);
        tmp_normal_to_surface[i].set_prec(NUMBITS);
    }
    //-

    //+ If the beam is not captured in the capillary - we are looking for the other beam if recording_not_captured == false
    // else save to base and the work is done.
    //+ Если пучок не захватился, мы ищем другой пучок при условии, что recording_not_captured == false
    // если recording_not_captured == true мы записываем как есть
    bool flag_captured = false;
    for (int i = 0; !flag_captured; ++i)
    {
        if (i > number_of_attempts)
        {
            print("ERROR: i > number_of_attempts");
            return -1;
        }
        source.get_random_x_y_z(point_source);
        source.get_random_phi_theta(directing_vector, phi, theta); // пока не будем сохранять углы источника
#ifdef DEBUG
        qDebug() << "Send Ray 0: point_source:" <<  point_source[0].get_d() << point_source[1].get_d() << point_source[2].get_d();
        qDebug() << "Send Ray 0: directing_vector:" <<  directing_vector[0].get_d() << directing_vector[1].get_d() << directing_vector[2].get_d();
#endif
        int tmp_result = linalg.NewtonMetodForSurfaceAndStraightLine(surface_lower_edge_of_cap
                                                                     , surface_lower_edge_of_cap
                                                                     , point_source
                                                                     , tmp_result_point
                                                                     , directing_vector
                                                                     , accuracy);
        //qDebug() << "123" << getQString(eval_surface.getValue(tmp_result_point[0], tmp_result_point[1], tmp_result_point[2])) << (eval_surface.getValue(tmp_result_point[0], tmp_result_point[1], tmp_result_point[2]) < ZERO)
//                << getQString(tmp_result_point[0]) << getQString(tmp_result_point[1]) << getQString(tmp_result_point[2]);
        if (tmp_result != 0)
        {
            print("WARNING: beam could not reach the bottom of the capillary border!");
            continue;
        }
        else if (eval_surface.getValue(tmp_result_point[0], tmp_result_point[1], tmp_result_point[2]) < ZERO)
        {
            //qDebug() << "123" << (eval_surface.getValue(tmp_result_point[0], tmp_result_point[1], tmp_result_point[2]) < ZERO);
            flag_captured = true;
        }
        else
        {
            flag_captured = false;
        }

        if (!flag_captured && recording_not_captured)
        {
            // записываем как есть!
            //+++++++++++++++RAY+++++++++++++++++++++++++
            QSqlQuery ray_query(dbase);
            ray_query.prepare("INSERT INTO rays (sources_id, caps_id, phi, theta, x_source, y_source, z_source, captured, reached_screen)"
                                          "VALUES (:sources_id, :caps_id, :phi, :theta, :x_source, :y_source, :z_source, :captured, :reached_screen);");
            ray_query.bindValue(":sources_id", source_id[0]);
            ray_query.bindValue(":caps_id", cap_id[0]);
            ray_query.bindValue(":phi", QString::fromStdString(getString(phi)));
            ray_query.bindValue(":theta", QString::fromStdString(getString(theta)));
            ray_query.bindValue(":x_source", QString::fromStdString(getString(point_source[0])));
            ray_query.bindValue(":y_source", QString::fromStdString(getString(point_source[1])));
            ray_query.bindValue(":z_source", QString::fromStdString(getString(point_source[2])));
            ray_query.bindValue(":captured", 0);
            ray_query.bindValue(":reached_screen", 0);
            ray_query.exec();
            QVariant ray_id = ray_query.lastInsertId();
            //---------------------------------------------
            //++++++++++++++++POINT++++++++++++++++++++++++
            QSqlQuery point_query(dbase);
            point_query.prepare("INSERT INTO points (rays_id, number_in_ray, x, y, z, angle, on_source, on_screen)"
                                    "VALUES (:rays_id, :number_in_ray, :x, :y, :z, :angle, :on_source, :on_screen);");
            point_query.bindValue(":rays_id", ray_id);
            point_query.bindValue(":number_in_ray", 0);
            point_query.bindValue(":x", QString::fromStdString(getString(point_source[0])));
            point_query.bindValue(":y", QString::fromStdString(getString(point_source[1])));
            point_query.bindValue(":z", QString::fromStdString(getString(point_source[2])));
            point_query.bindValue(":angle", ""); //"0."
            point_query.bindValue(":on_source", 1);
            point_query.bindValue(":on_screen", 0);
            point_query.exec();
            //---------------------------------------------
            print("@@@ OK: not_captured!");
#ifdef DEBUG
            output("@@@ OK: not_captured!");
#endif
            return 0;
        }

    }
    if (!flag_captured)
    {
        print("ERROR: Send ray: Unknown error");
        return -1;
    }
    else
    {
#ifdef DEBUG
        qDebug() << "Send Ray 0.1: tmp_result_point:" <<  tmp_result_point[0].get_d() << tmp_result_point[1].get_d() << tmp_result_point[2].get_d() << eval_surface.Calc(tmp_result_point[0], tmp_result_point[1], tmp_result_point[2]).get_d()
                << (eval_surface.Calc(tmp_result_point[0], tmp_result_point[1], tmp_result_point[2]) < ZERO) << (tmp_result == 0);
        qDebug() << "Send Ray 1: tmp_result_point:" <<  tmp_result_point[0].get_d() << tmp_result_point[1].get_d() << tmp_result_point[2].get_d();
#endif
    }
    //-

    // луч захватился, записываем точку на источнике
    // save source point to DB:
    //+++++++++++++++RAY+++++++++++++++++++++++
    QSqlQuery ray_query(dbase);
    ray_query.prepare("INSERT INTO rays (sources_id, caps_id, phi, theta, x_source, y_source, z_source, captured, reached_screen)"
                                  "VALUES (:sources_id, :caps_id, :phi, :theta, :x_source, :y_source, :z_source, :captured, :reached_screen);");
    ray_query.bindValue(":sources_id", source_id[0]);
    ray_query.bindValue(":caps_id", cap_id[0]);
    ray_query.bindValue(":phi", QString::fromStdString(getString(phi)));
    ray_query.bindValue(":theta", QString::fromStdString(getString(theta)));
    ray_query.bindValue(":x_source", QString::fromStdString(getString(point_source[0])));
    ray_query.bindValue(":y_source", QString::fromStdString(getString(point_source[1])));
    ray_query.bindValue(":z_source", QString::fromStdString(getString(point_source[2])));
    ray_query.bindValue(":captured", 1);
    ray_query.bindValue(":reached_screen", 0);
    ray_query.exec();
    QVariant ray_id = ray_query.lastInsertId();
    //-------------------------------------------
    //+++++++++++++++++++++++++++++++++++++++++++
    QSqlQuery point_query(dbase);
    point_query.prepare("INSERT INTO points (rays_id, number_in_ray, x, y, z, angle, on_source, on_screen)"
                            "VALUES (:rays_id, :number_in_ray, :x, :y, :z, :angle, :on_source, :on_screen);");
    point_query.bindValue(":rays_id", ray_id);
    point_query.bindValue(":number_in_ray", 0);
    point_query.bindValue(":x", QString::fromStdString(getString(point_source[0])));
    point_query.bindValue(":y", QString::fromStdString(getString(point_source[1])));
    point_query.bindValue(":z", QString::fromStdString(getString(point_source[2])));
    point_query.bindValue(":angle", "");//"0."
    point_query.bindValue(":on_source", 1);
    point_query.bindValue(":on_screen", 0);
    point_query.exec();
    //--------------------------------------------

    // looking for a first point (and other) of intersection line with the capillary
    for (int i = 0; i < sizeColumns; ++i)
    {
        prev_point[i] = point_source[i];
    }
    bool flag_ray_on_screen = false;
    bool flag_ray_departed = false;
    int number_point = 1;
    for (; ;++number_point)
    {
        if (number_point > number_of_attempts)
        {
            print("ERROR: Send ray: number_point > number_of_attempts");
            return -1;
        }
        int tmp_result = linalg.NewtonMetodForSurfaceAndStraightLine(surface_of_cap
                                                                     , surface_of_cap
                                                                     , prev_point
                                                                     , tmp_result_point
                                                                     , directing_vector
                                                                     , accuracy);
        if (tmp_result != 0)
        {
            return -1;
        }
        //+ if the beam flew in the wrong direction:
        // eval_lower.getValue(tmp_result_point[0], tmp_result_point[1], tmp_result_point[2]) < ZERO --- for ane point and for first
        // or tmp_wrong += (tmp_result_point[j] - prev_point[j]) * directing_vector[j]; tmp_wrong < ZERO
        REAL tmp_wrong("0.0", NUMBITS);
        for (int j = 0; j < sizeColumns; ++j)
        {
            tmp_wrong += (tmp_result_point[j] - prev_point[j]) * directing_vector[j];
        }
//        qDebug() << "003" << (eval_lower.getValue(tmp_result_point[0], tmp_result_point[1], tmp_result_point[2]) < ZERO) << (tmp_wrong < ZERO) << getQString(tmp_result_point[0]) << getQString(tmp_result_point[1]) << getQString(tmp_result_point[2])
//                << getQString(prev_point[0]) << getQString(prev_point[1]) << getQString(prev_point[2]);
        while (tmp_wrong < ZERO || eval_lower.getValue(tmp_result_point[0], tmp_result_point[1], tmp_result_point[2]) < ZERO)
        {
//            qDebug() << "004";
            //construct surface_of_cap_modified
            if (tmp_result_point[2] >= ZERO)
            {
                surface_of_cap_modified = "(" + surface_of_cap_modified + ")/(z-" + getString(tmp_result_point[2]).data() + ")";
            }
            else
            {
                surface_of_cap_modified = "(" + surface_of_cap_modified + ")/(z+" + getString(abs(tmp_result_point[2])).data() + ")";
            }
            // some const
            tmp_wrong += ut.getRandom() * 1000.0;
            for(int j = 0; j < sizeColumns; ++j)
            {
                tmp_result_point[j] = ut.getRandom() * 200000.0 - 100000.0;
            }
//            qDebug() << "Send Ray 1.2: rPoint[2] < pointsZ[0]:" << "surface_of_cap_modified:" << surface_of_cap_modified.data();
            tmp_result = linalg.NewtonMetodForSurfaceAndStraightLine(surface_of_cap
                                                                     , surface_of_cap_modified
                                                                     , prev_point
                                                                     , tmp_result_point
                                                                     , directing_vector
                                                                     , accuracy);
            if (tmp_result != 0)
            {
                qDebug() << "ERROR: Send Ray 1.3: continue";
                return -1;
            }
            tmp_wrong = "0.0";
            for (int j = 0; j < sizeColumns; ++j)
            {
                tmp_wrong += (tmp_result_point[j] - prev_point[j]) * directing_vector[j];
            }
        }
        //- if the beam flew in the wrong direction
        //+ if the beam back to the same point
        for (int j = 0; abs(prev_point[0] - tmp_result_point[0]) <= accuracy_phys
            && abs(prev_point[1] - tmp_result_point[1]) <= accuracy_phys
            && abs(prev_point[2] - tmp_result_point[2]) <= accuracy_phys; ++j)
        {
            if (j > number_of_attempts)
            {
                print("ERROR: Send Ray: Failed to find the second point of intersection of the beam. capillary has a hole?");
                return -1;
            }
            REAL tmp_rnd1("-1e10", NUMBITS), tmp_rnd2("1e10", NUMBITS);
            //??
            // the largest interval is observed in the z-axis in our geometry
            // just in case, take further (*10^10)
            tmp_rnd1 = ZERO - abs(point_source[2]) * tmp_rnd1;
            tmp_rnd2 = abs(z_upper_cap) * tmp_rnd2;
//            qDebug() << "008" << j << surface_of_cap_modified;
            for (int j = 0; j < sizeColumns; ++j)
            {
                tmp_result_point_check[j] = ut.getRandom(tmp_rnd1, tmp_rnd2); // new initial approximation
            }
            int tmp_result = linalg.NewtonMetodForSurfaceAndStraightLine(surface_of_cap
                                                                         ,surface_of_cap_modified
                                                                         ,prev_point
                                                                         ,tmp_result_point_check
                                                                         ,directing_vector
                                                                         ,accuracy);
            if (tmp_result != 0)
            {
                print("ERROR: Send Ray 2.2: continue");
                continue;
            }
            for (int j = 0; j < sizeColumns; ++j)
            {
                tmp_result_point[j] = tmp_result_point_check[j];
            }
#ifdef DEBUG
            qDebug() << (abs(prev_point[0] - tmp_result_point[0]) <= accuracy_phys
                    && abs(prev_point[1] - tmp_result_point[1]) <= accuracy_phys
                    && abs(prev_point[2] - tmp_result_point[2]) <= accuracy_phys)
                    << get_qstr(abs(prev_point[0] - tmp_result_point[0]))
                    << get_qstr(abs(prev_point[1] - tmp_result_point[1]))
                    << get_qstr(abs(prev_point[2] - tmp_result_point[2]))
                    << get_qstr(accuracy_phys);
#endif
        }
        //- if the beam back to the same point
#ifdef DEBUG
        qDebug() << "!!!!!paranoid_check: 009" << paranoid_check;
#endif
//        qDebug() << "009";
        if (paranoid_check)
        {
            //+теперь надо проверить, что между prev_point и tmp_result_point нет ещё одной точки - это очень важная проверка
            //будем искать до тех пор, пока не сломается NewtonMetodForSurfaceAndStraightLine
            bool continue_checking = true;
            REAL tmp_vecotor_a("0.0", NUMBITS);
            REAL tmp_vecotor_b("0.0", NUMBITS);
            for (int j = 0; j < sizeColumns; ++j)
            {
                tmp_result_point_check[j] = tmp_result_point[j];
            }
            while (continue_checking)
            {
                if(tmp_vecotor_a > ZERO && tmp_vecotor_b < ZERO)
                {
                    //эта точка находится между prev_point и tmp_result_point
                    qDebug() << "НИЧЕГО СЕБЕ!!!";
                    tmp_result_point[0] = tmp_result_point_check[0];
                    tmp_result_point[1] = tmp_result_point_check[1];
                    tmp_result_point[2] = tmp_result_point_check[2];
                    surface_of_cap_modified = "(" + surface_of_cap_modified + ")/(z-(" + getString(tmp_result_point[2]).data() + "))";
                }
                else
                {
                    surface_of_cap_modified = "(" + surface_of_cap_modified + ")/(z-(" + getString(tmp_result_point_check[2]).data() + "))";
                }
                tmp_result = linalg.NewtonMetodForSurfaceAndStraightLine(surface_of_cap
                                                                         , surface_of_cap_modified
                                                                         , prev_point
                                                                         , tmp_result_point_check
                                                                         , directing_vector
                                                                         , accuracy);
                if (tmp_result != 0)
                {
                    continue_checking = false;
                    // tmp_result_point is truth point!
                    break;
                }
                for (int j = 0; j < sizeColumns; ++j)
                {
                    tmp_vecotor_a += (tmp_result_point_check[j] - prev_point[j]) * directing_vector[j];
                    tmp_vecotor_b += (tmp_result_point_check[j] - tmp_result_point[j]) * directing_vector[j];
                }
                qDebug() << "@@@&&&@@@";
            }
        }
        //-
//        qDebug() << "010";
        // Point is found now check that the beam hit the screen. Set a new approach and new direction vector!
        if (eval_upper.getValue(tmp_result_point[0], tmp_result_point[1], tmp_result_point[2]) > ZERO)//попал на экран
        {
            flag_ray_on_screen = true;
            break;
        }
//        qDebug() << "011_" << getQString(tmp_result_point[0]) << getQString(tmp_result_point[1]) << getQString(tmp_result_point[2]);
        if (eval_lower.getValue(tmp_result_point[0], tmp_result_point[1], tmp_result_point[2]) < ZERO)//развернулся обратно к источнику
        {
            flag_ray_departed = true;
            break;
        }
//        qDebug() << "011" << surface_of_cap_modified << surface_of_cap;
//        qDebug() << "012";
        //+ разворачиваем направляющий вектор
        for (int j = 0; j < sizeColumns; ++j)
        {
            tmp_normal_to_surface[j] = eval_surface.getValueDerivative(namesOfVar[j], tmp_result_point[0], tmp_result_point[1], tmp_result_point[2]);
        }
        REAL tmp("0.0", NUMBITS), tmp_projection("0.0", NUMBITS);
        tmp = sqrt(tmp_normal_to_surface[0] * tmp_normal_to_surface[0] +
                tmp_normal_to_surface[1] * tmp_normal_to_surface[1] +
                tmp_normal_to_surface[2] * tmp_normal_to_surface[2]);
        for (int j = 0; j < sizeColumns; ++j)
        {
            tmp_normal_to_surface[j] /= tmp;  // нормированный вектор внутренней нормали
        }
//        qDebug() << "013";
        tmp_projection =   tmp_normal_to_surface[0] * directing_vector[0]
                + tmp_normal_to_surface[1] * directing_vector[1]
                + tmp_normal_to_surface[2] * directing_vector[2]; // проекция прошлого направл. вектора на нормаль
        tmp = sqrt(directing_vector[0] * directing_vector[0] +
                directing_vector[1] * directing_vector[1] +
                directing_vector[2] * directing_vector[2]);
        // calculate glancing angle
        QByteArray error;
        angle = _asin(tmp_projection / tmp, ZERO, error); //angle = asin(tmp_projection / tmp);
        //+++++++++++++++++POINT1++++++++++++++++++++++++++
        point_query.clear();
        point_query.prepare("INSERT INTO points (rays_id, number_in_ray, x, y, z, angle, on_source, on_screen)"
                                "VALUES (:rays_id, :number_in_ray, :x, :y, :z, :angle, :on_source, :on_screen);");
        point_query.bindValue(":rays_id", ray_id);
        point_query.bindValue(":number_in_ray", number_point);
        point_query.bindValue(":x", QString::fromStdString(getString(tmp_result_point[0])));
        point_query.bindValue(":y", QString::fromStdString(getString(tmp_result_point[1])));
        point_query.bindValue(":z", QString::fromStdString(getString(tmp_result_point[2])));
        point_query.bindValue(":angle", QString::fromStdString(getString(angle)));
        point_query.bindValue(":on_source", 0);
        point_query.bindValue(":on_screen", 0);
        point_query.exec();
        //-------------------------------------------------
//        qDebug() << "014";
        directing_vector[0] = directing_vector[0] - 2.0 * abs(tmp_projection) * tmp_normal_to_surface[0]; //новый направляющий вектор прямой)
        directing_vector[1] = directing_vector[1] - 2.0 * abs(tmp_projection) * tmp_normal_to_surface[1]; //новые = для новой прямой
        directing_vector[2] = directing_vector[2] - 2.0 * abs(tmp_projection) * tmp_normal_to_surface[2]; //для старой прямой значение потеряно и не нужно

        if (angle < ZERO) //??
        {
            qDebug() << "### ERROR: ANGLEGLACING < 0!";
            return -1;
        }
//        qDebug() << "015";
        for (int j = 0; j < sizeColumns; ++j)
        {
            prev_point[j] = tmp_result_point[j];     // new prev point
        }
        for (int j = 0; j < sizeColumns; ++j)
        {
            tmp_result_point[j] += directing_vector[j]; // new initial approximation
        }
//        qDebug() << "Send Ray 2.5: tmp_result_point:" <<  tmp_result_point[0].get_d() << tmp_result_point[1].get_d() << tmp_result_point[2].get_d();
        //+возвращаем поверхность на место
//        qDebug() << "111" << surface_of_cap_modified << "###" << surface_of_cap;
        surface_of_cap_modified.clear();
        surface_of_cap_modified.append(surface_of_cap);
//        qDebug() << "111" << surface_of_cap_modified << "###" << surface_of_cap;
        //-
    }
    if (flag_ray_on_screen)
    {
        // ищем точку на экране и её записываем в базу
        int tmp_result = linalg.NewtonMetodForSurfaceAndStraightLine(surface_upper_edge_of_cap  // !!!
                                                                     ,surface_upper_edge_of_cap // !!!
                                                                     ,prev_point
                                                                     ,tmp_result_point
                                                                     ,directing_vector
                                                                     ,accuracy);
        if (tmp_result != 0)
        {
            qDebug() << "ERROR: Send Ray 2.1";
            return -1;
        }
        //++++++++++++++POINT++++++++++++++++++++++++
        point_query.clear();
        point_query.prepare("INSERT INTO points (rays_id, number_in_ray, x, y, z, angle, on_source, on_screen)"
                                "VALUES (:rays_id, :number_in_ray, :x, :y, :z, :angle, :on_source, :on_screen);");
        point_query.bindValue(":rays_id", ray_id);
        point_query.bindValue(":number_in_ray", number_point);
        point_query.bindValue(":x", QString::fromStdString(getString(tmp_result_point[0])));
        point_query.bindValue(":y", QString::fromStdString(getString(tmp_result_point[1])));
        point_query.bindValue(":z", QString::fromStdString(getString(tmp_result_point[2])));
        point_query.bindValue(":angle", "");//getQString(angle));
        point_query.bindValue(":on_source", 0);
        point_query.bindValue(":on_screen", 1);
        point_query.exec();
        //-------------------------------------------
        //+++++++++++++++++RAY+++++++++++++++++++++++
        ray_query.clear();
        ray_query.prepare("UPDATE rays SET reached_screen = 1 WHERE ID = ?");
        ray_query.addBindValue(ray_id);
        ray_query.exec();
        //-------------------------------------------
    }
    else if (flag_ray_departed)
    {
        // nothing to do
    }
    else
    {
        print("ERROR: Send ray: Unknown error!");
        return -1;
    }
//    qDebug() << "@@@ OK: Send Ray: (flag_ray_departed, flag_ray_on_screen)" <<  flag_ray_departed << flag_ray_on_screen;

    return 0;
}

REAL XRays::getCoefOfRefl(REAL theta, REAL nu, REAL plasmaFrequency)
{
    QByteArray error;
    REAL result("0.0", NUMBITS);
    REAL sinTheta("0.0", NUMBITS), cosTheta("0.0", NUMBITS);
    sinTheta = _sin(theta, ZERO, error);
    cosTheta = _cos(theta, ZERO, error);

    if (plasmaFrequency == ZERO)
    {
        plasmaFrequency = 7.2539680435955e6; //GHz
    }
    complex_mpf epsilonCap(ZERO, ZERO), tmp2sin(sinTheta, ZERO), tmp2cos(cosTheta, ZERO);
    epsilonCap.imag = "1e-7";
    complex_mpf tmp((1 - _pow(plasmaFrequency / nu, 2, error)), epsilonCap.imag);
    tmp = tmp - tmp2cos.pow2();
    complex_mpf tempComplSqrt = tmp.sqrt_compl();
//    qDebug() << "sqrt";
    tempComplSqrt.show();
//    qDebug() << "sin";
    tmp2sin.show();
    complex_mpf tmp3 = (tmp2sin - tempComplSqrt);
    complex_mpf tmp4 = (tmp2sin + tempComplSqrt);
//    qDebug() << "!-";
    tmp3.show();
//    qDebug() << "!+";
    tmp4.show();
    complex_mpf tmp2 = (tmp2sin - tempComplSqrt) / (tmp2sin + tempComplSqrt);
    tmp2.show();
//    qDebug() << getQString(tmp2.mod());
    result = _pow(tmp2.mod(), 2, error);
    return result;
  //  qDebug() << tempComplSqrt.real() << tempComplSqrt.imag();
//    qDebug() << "nu:" << nu << "plasmaFrequency/nu:" << plasmaFrequency/nu << "angle theta:" << theta;
//    qDebug() << "nu*4.135667517e-6:" << (nu*4.135667517e-6) << "sqrt(coef):" << std::abs((tempsin - tempComplSqrt)/(tempsin + tempComplSqrt));
//    return (_pow(abs( (tempsin - tempComplSqrt)/(tempsin + tempComplSqrt) ),2));
}

double XRays::getCoefOfRefl(double theta, double nu)
{
    double plasmaFrequency = 7.2539680435955e6;
    double epsilonCap = 1e-7;

    std::complex<double> tmp2sin = std::complex<double>(std::sin(theta), 0), temp2cos = std::complex<double>(std::cos(theta) , 0);
    std::complex<double> tmp = std::complex<double>(1 - std::pow(plasmaFrequency / nu, 2), epsilonCap);
    tmp = tmp - std::pow(temp2cos ,2);
    tmp = std::sqrt(tmp);
//    qDebug() << "sqrt:" << QString::number(tmp.real(),'g', 15) << QString::number(tmp.imag(),'g', 15);
//    qDebug() << "sin:" << QString::number(tmp2sin.real(),'g', 15) << QString::number(tmp2sin.imag(),'g', 15);
//    qDebug() << "!:" << QString::number((tmp2sin - tmp).real(),'g', 15) << QString::number((tmp2sin - tmp).imag(),'g', 15);
//    qDebug() << "!:" << QString::number((tmp2sin + tmp).real(),'g', 15) << QString::number((tmp2sin + tmp).imag(),'g', 15);
    std::complex<double> tmp2 = (tmp2sin - tmp) / (tmp2sin + tmp);
//    qDebug() << "tmp2:" << QString::number(tmp2.real(),'g', 15) << QString::number(tmp2.imag(),'g', 15) << QString::number(tmp2sin.real(),'g', 15);
    tmp2 = std::abs(tmp2);
//    qDebug() << "abs:" << tmp2.real() << tmp2.imag();
    std::complex<double> tempComplSqrt = std::sqrt(
                std::complex<double>(1.0 - std::pow(plasmaFrequency/nu,2) , epsilonCap) - std::pow(temp2cos ,2)
                );


  //  qDebug() << tempComplSqrt.real() << tempComplSqrt.imag();
//    double tempsin = std::sin(theta);
//    qDebug() << "nu:" << nu << "plasmaFrequency/nu:" << plasmaFrequency/nu << "angle theta:" << theta;
//    qDebug() << "nu*4.135667517e-6:" << (nu*4.135667517e-6) << "sqrt(coef):" << std::abs((tempsin - tempComplSqrt)/(tempsin + tempComplSqrt));
    return (std::pow(std::abs( (tmp2sin - tempComplSqrt)/(tmp2sin + tempComplSqrt) ),2));
}
