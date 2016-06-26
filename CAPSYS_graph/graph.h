#ifndef GRAPH_H
#define GRAPH_H

#include <QWidget>
#include <QtSql>
#include <mgl2/qt.h>
#include "../settings.h"
#include "../eval_mpf.h"
#include "../dialogsetranges.h"


class Graph : public QWidget, public mglDraw
{
    Q_OBJECT
    QSqlDatabase dbase;
    DialogSetRanges *dialogranges;
    int capId;
    int sourceId;
    int countTestRays;

    bool open_database();

    double xLowerLimit, xUpperLimit, yLowerLimit, yUpperLimit, zLowerLimit, zUpperLimit;
    double rotateX, rotateY, rotateZ;
public:
    explicit Graph(QWidget *parent = 0);
    int Draw(mglGraph *gr);
signals:

public slots:
    void slotSetRanges();
    void slotSetCapId(int cap_id) { capId = cap_id; }
    void slotSetSourceId(int source_id) { sourceId = source_id; }
    void slotSetCountRays(int count_rays) { countTestRays = count_rays; }
};
#endif // GRAPH_H
