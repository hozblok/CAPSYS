#include "graph.h"

Graph::Graph(QWidget *parent) :
    QWidget(parent), mglDraw()
{
    dbase = QSqlDatabase::addDatabase("QSQLITE", "graph");
    dbase.setDatabaseName("capsys.sqlite");
    if (!dbase.open()) {
        QString tmp = (QString) "ERROR: open_database: Unable to connect to database!" + dbase.lastError().text();
        output(tmp.toUtf8().data());
    }

    dialogranges = new DialogSetRanges(this);
    capId = 1;
    sourceId = 1;
    countTestRays = 0;

    QObject::connect(dialogranges, SIGNAL(rejected()), this, SLOT(slotSetRanges()));
}

bool Graph::open_database()
{
    if(!dbase.isOpen())
    {
        if (!dbase.open()) {
            QString tmp = (QString) "ERROR: open_database: Unable to connect to database!" + dbase.lastError().text();
            output(tmp.toUtf8().data());
            return false;
        }
    }
    return true;
}


int Graph::Draw(mglGraph *gr)
{
    if (capId < 1 || sourceId < 1)
    {
        return -1;
    }
    if(!open_database())
    {
        return -1;
    }

    //+ get cap from bd
    QSqlQuery query_cap(dbase);
    query_cap.prepare("SELECT id, surface, dimension_m, accuracy, accuracy_physics, lower_edge, upper_edge FROM caps WHERE id = ?");
    query_cap.addBindValue(capId);
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
    }
    else
    {
        output("ERROR: send_ray: caps no found!");
        return -1;
    }
    //-

    //+ get source from bd
    QSqlQuery query_source(dbase);
    query_source.prepare("SELECT id, surface, x_lower, x_upper, y_lower, y_upper, z_lower, z_upper, phi_lower, phi_upper, theta_lower, theta_upper FROM sources WHERE id = ?");
    query_source.addBindValue(sourceId);
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
    }
    else
    {
        output("ERROR: send_ray: source no found!");
        return -1;
    }
    //-

    //+ initialization data
    REAL accuracy(accur.constData(), NUMBITS);
    REAL accuracy_phys(accur_phys.constData(), NUMBITS);
    Eval eval_surface(surface_of_cap);
    Eval eval_lower(surface_lower_edge_of_cap);
    Eval eval_upper(surface_upper_edge_of_cap);
    Eval eval_surface_source(surface_source);
    REAL z_upper_cap("0.0", NUMBITS);
    z_upper_cap = abs(eval_upper.getValue(ZERO, ZERO, ZERO));
    REAL z_lower_cap(z_lower.constData(), NUMBITS);

//    Source source(surface_source, x_lower, x_upper
//                  , y_lower, y_upper, z_lower, z_upper
//                  , phi_lower, phi_upper, theta_lower, theta_upper);

    dialogranges->setParams(-2,2,
                            -2,2,
                            z_lower_cap.get_d() - 10, z_upper_cap.get_d() + 10);
    dialogranges->exec();


    const char *colors[] = {"{e7}","{n7}","{g7}","{y7}","{q7}","{c7}","{m7}"};
    gr->SetDefScheme("Nn{c4}{e9}qrR");
//    gr->SetSize(1024,768);
    gr->SetFontSizePT(7);
    QByteArray lengthUnit = "";
    int dimension_m = -5;
    if (dimension_m == -6)
    {
        lengthUnit = "\\mu m";
    }
    else if (dimension_m == -5)
    {
        lengthUnit = "10\\cdot\\mu m";
    }
    else if (dimension_m == -4)
    {
        lengthUnit = "0.1 mm";
    }
    else if (dimension_m == -3)
    {
        lengthUnit = "mm";
    }
    else if (dimension_m == -2)
    {
        lengthUnit = "sm";
    }
    else if (dimension_m == -1)
    {
        lengthUnit = "10^-1\\cdotm";
    }
    else if (dimension_m == 0)
    {
        lengthUnit = "m";
    }
    else if (dimension_m == 1)
    {
        lengthUnit = "10\\cdotm";
    }
    else if (dimension_m == 2)
    {
        lengthUnit = "10^2\\cdotm";
    }
    else if (dimension_m == 3)
    {
        lengthUnit = "km";
    }

    int flagDraw = 0;
    if (flagDraw == 1)
    {
        qDebug() << "asdasd";
        gr->Rotate(rotateX, rotateZ, rotateY);
        gr->Alpha(true);
        gr->SetRanges(-1, 1, -1, 1, -5, 202);

        mglData grtemp(100,100,100);
//        for(i = 0; i < dataX.size(); ++i)
//        {
//            int countPoints = dataX[i].size();

//            mglData x(countPoints), y(countPoints), z(countPoints);
//            for (j= 0; j < countPoints; ++j)
//            {
//                x.a[j] = dataX[i][j];
//                y.a[j] = dataY[i][j];
//                z.a[j] = dataZ[i][j];
//                qDebug() << x.a[j] << y.a[j] << z.a[j] << j << i;
//            }
//            gr->Plot(x,y,z,"i");
//        }

        QByteArray botbot = "<0";
        QByteArray endend = ">0";

        for(int i = 0; i < 1; ++i)
        {
            gr->CutOff("(z-0<0) | (z-200>0)");
            gr->Fill(grtemp, "((x-100000)^2+y^2+z^2+10000000000-0.01)^2-40000000000*((x-100000)^2+z^2)", "");
//            gr->Fill(grtemp, "(x-(sin(1/800)*z)^2)^2+(y)^2 - 0.000001", "");
            try {
                gr->Surf3(0.0,grtemp,colors[2],"");
                gr->CutOff("");
            } catch (...) {
                gr->Surf3(0.0,grtemp,"i","");
                gr->CutOff("");
            }
            gr->CutOff("");

        }

        for(int i = 0; i < 1; ++i)
        {

            gr->Fill(grtemp,"(z+x*1000-200)","");
            gr->Surf3(0.0,grtemp,"c","");
        }

//        gr->Label('x',"x, "+lengthUnit,1);
//        gr->Label('y',"y, "+lengthUnit,1);
//        gr->Label('z',"z, "+lengthUnit,1);
        gr->Label('x',"x",1);
        gr->Label('y',"y",1);
        gr->Label('z',"z",1);
        gr->Box(); gr->Axis(); gr->Grid("xyz");

        gr->WritePNG(QByteArray("00-test___Geom-__.png").data());
    }
    if (flagDraw == 0)
    {
        //--------------------------------------------------------
        QVector< QVector<double> > dataX, dataY, dataZ;
        QVector< double > tempX, tempY, tempZ;

        QSqlQuery query_rays(dbase);
        query_rays.prepare("SELECT id FROM rays WHERE caps_id = ? AND sources_id = ? AND captured = 1 AND reached_screen = 1 LIMIT ?");
        query_rays.addBindValue(capId);
        query_rays.addBindValue(sourceId);
        query_rays.addBindValue(countTestRays);
        query_rays.exec();
        qDebug() << capId << sourceId << countTestRays << query_rays.executedQuery();
        while (query_rays.next())
        {
            int id_ray = query_rays.value("id").toInt();
            qDebug() << "@@@@" << id_ray;
            QSqlQuery query_points(dbase);
            query_points.prepare("SELECT x,y,z FROM points WHERE rays_id = ?");
            query_points.addBindValue(id_ray);
            query_points.exec();
            tempX.clear();
            tempY.clear();
            tempZ.clear();
            while (query_points.next())
            {
                REAL x_(query_points.value("x").toByteArray().constData(), NUMBITS);
                REAL y_(query_points.value("y").toByteArray().constData(), NUMBITS);
                REAL z_(query_points.value("z").toByteArray().constData(), NUMBITS);

                tempX.push_back(x_.get_d());
                tempY.push_back(y_.get_d());
                tempZ.push_back(z_.get_d());
            }
            dataX.push_back(tempX);
            dataY.push_back(tempY);
            dataZ.push_back(tempZ);
        }
        qDebug() << "dataX" << dataX << dataY << dataZ;

        gr->Rotate(rotateX, rotateZ, rotateY);
        gr->Alpha(true);
        gr->SetRanges(xLowerLimit, xUpperLimit, yLowerLimit, yUpperLimit, zLowerLimit, zUpperLimit);

        mglData grtemp(100,100,100);
        for(int i = 0; i < dataX.size(); ++i)
        {
            int countPoints = dataX[i].size();

            mglData x(countPoints), y(countPoints), z(countPoints);
            for (int j= 0; j < countPoints; ++j)
            {
                x.a[j] = dataX[i][j];
                y.a[j] = dataY[i][j];
                z.a[j] = dataZ[i][j];
                qDebug() << x.a[j] << y.a[j] << z.a[j] << j << i;
            }
            gr->Plot(x,y,z,"i");
        }

        QByteArray botbot = "<0";
        QByteArray endend = ">0";

        for(int i = 0; i < 1; ++i)
        {
            gr->CutOff(("(" + surface_lower_edge_of_cap + botbot + ") | (" + surface_upper_edge_of_cap + endend + ")").data());
            gr->Fill(grtemp, surface_of_cap.data(), "");
            //            gr->Surf3(0.0,grtemp,colors[i],"");
            try {
                gr->Surf3(0.0,grtemp,colors[i],"");
                gr->CutOff("");
            } catch (...) {
                gr->Surf3(0.0,grtemp,"i","");
                gr->CutOff("");
            }
            gr->CutOff("");

        }

        for(int i=0; i < 1; ++i)
        {

            gr->Fill(grtemp,surface_upper_edge_of_cap,"");
            gr->Surf3(0.0,grtemp,"c","");
        }

        gr->Label('x',"x, "+lengthUnit,1);
        gr->Label('y',"y, "+lengthUnit,1);
        gr->Label('z',"z, "+lengthUnit,1);
        gr->Box(); gr->Axis(); gr->Grid();

        gr->WritePNG(QByteArray("00-testGeom-__.png").data());
    }

    return 0;
}

void Graph::slotSetRanges()
{
    xLowerLimit = dialogranges->getXLower();
    yLowerLimit = dialogranges->getYLower();
    zLowerLimit = dialogranges->getZLower();
    xUpperLimit = dialogranges->getXUpper();
    yUpperLimit = dialogranges->getYUpper();
    zUpperLimit = dialogranges->getZUpper();
    rotateX = dialogranges->getRx();
    rotateY = dialogranges->getRy();
    rotateZ = dialogranges->getRz();
}
