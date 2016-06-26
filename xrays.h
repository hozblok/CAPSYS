#ifndef XRAYS_H
#define XRAYS_H

#include <QObject>
#include <QtSql>
#include "settings.h"
#include "linalg.h"
#include "source.h"
#include "complex_mpf.h"
#include <complex>

class XRays : public QObject
{
    Q_OBJECT
private:
    Utils ut;
    LinAlg linalg;
    QSqlDatabase dbase;
    bool open_database();

public:
    explicit XRays(QObject *parent = 0, QString connectionName = "connection");

    int send_ray(QVector<int> &cap_id,
                 QVector<int> &source_id,
                 bool recording_not_captured = true, //хорошо работает
                 int number_of_attempts = 300,
                 bool paranoid_check = false); //не оттестировано

    REAL getCoefOfRefl(REAL theta, REAL nu, REAL plasmaFrequency = ZERO);
    double getCoefOfRefl(double theta, double nu);
signals:

public slots:

};

#endif // XRAYS_H
