#include "capillary.h"


Capillary::Capillary(int sourceId) :
    id(sourceId),
    dimensionM(-6),
    surface(0),
    lowerEdge(0),
    upperEdge(0),
    accuracy("0.0", NUMBITS),
    accuracyPhysics("0.0", NUMBITS)
{
    print((QString("#creating Capillary, id:") + QString::number(id)).toLocal8Bit().constData(), true);

    // get capillary
    QSqlQuery query;
    query.prepare("SELECT surface_id, "
                  "dimension_m, "
                  "accuracy, "
                  "accuracy_physics, "
                  "surface_lower_edge_id, "
                  "surface_upper_edge_id, "
                  "comment "
                  "FROM caps WHERE id = ?");
    query.addBindValue(id);
    query.exec();
    if (query.next())
    {
        surface         = new Surface(query.value("surface_id").toInt());
        dimensionM      = query.value("dimension_m").toInt();
        accuracy        = query.value("accuracy").toByteArray().constData();
        accuracyPhysics = query.value("accuracy_physics").toByteArray().constData();
        lowerEdge       = new Surface(query.value("surface_lower_edge_id").toInt());
        upperEdge       = new Surface(query.value("surface_upper_edge_id").toInt());
        comment         = query.value("comment").toByteArray();
    }
    else
    {
        //?? вызвать исключение!
    }
}

Capillary::~Capillary()
{
    delete surface;
    delete lowerEdge;
    delete upperEdge;
    print((QString("#deleting Capillary, id:") + QString::number(id)).toLocal8Bit().constData(), true);
}

Surface *Capillary::getSurface() const
{
    return surface;
}

Surface *Capillary::getLowerEdge() const
{
    return lowerEdge;
}

Surface *Capillary::getUpperEdge() const
{
    return upperEdge;
}

const float100et *Capillary::getAccuracy() const
{
    return &accuracy;
}

const float100et *Capillary::getAccuracyPhysics() const
{
    return &accuracyPhysics;
}
