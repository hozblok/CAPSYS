#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QSqlQuery>
#include <QVector>
#include "settings.h"
#include "source.h"

class Project : public QObject
{
    Q_OBJECT

    int id;
    QVector<int> capsIds, sourcesIds, screensIds;
    QVector<int> numsOfBeams; //for each source
public:
    explicit Project(int projId, QObject *parent = 0);
    ~Project();

signals:

public slots:
    int startModelling();

};

#endif // PROJECT_H
