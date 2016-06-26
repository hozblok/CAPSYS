#ifndef DIALOGSETRANGES_H
#define DIALOGSETRANGES_H

#include <QDialog>

namespace Ui {
class DialogSetRanges;
}

class DialogSetRanges : public QDialog
{
    Q_OBJECT
    Ui::DialogSetRanges *ui;
    double xLowerLimit, xUpperLimit, yLowerLimit, yUpperLimit, zLowerLimit, zUpperLimit;
    double rotateX, rotateY, rotateZ; //amount of rotation about the axis
    
public:
    explicit DialogSetRanges(QWidget *parent = 0);
    ~DialogSetRanges();
    void setParams(const double _xLowerLimit = 0, const double _xUpperLimit = 0,
                   const double _yLowerLimit = 0, const double _yUpperLimit = 0,
                   const double _zLowerLimit = 0, const double _zUpperLimit = 0,
                   const double _rotateX = 0, const double _rotateY = 0, const double _rotateZ = 0);

    double getXLower() { return xLowerLimit; }
    double getYLower() { return yLowerLimit; }
    double getZLower() { return zLowerLimit; }
    double getXUpper() { return xUpperLimit; }
    double getYUpper() { return yUpperLimit; }
    double getZUpper() { return zUpperLimit; }
    double getRx()     { return rotateX; }
    double getRy()     { return rotateY; }
    double getRz()     { return rotateZ; }

protected:
    void changeEvent(QEvent *e);

private slots:
    void slotSetRotateX(double t)     { rotateX = t; }
    void slotSetRotateY(double t)     { rotateY = t; }
    void slotSetRotateZ(double t)     { rotateZ = t; }
    void slotSetXLower(double t) { xLowerLimit = t; }
    void slotSetYLower(double t) { yLowerLimit = t; }
    void slotSetZLower(double t) { zLowerLimit = t; }
    void slotSetXUpper(double t) { xUpperLimit = t; }
    void slotSetYUpper(double t) { yUpperLimit = t; }
    void slotSetZUpper(double t) { zUpperLimit = t; }

    void on_buttonBox_accepted();
    void on_pushButton_anglreset_clicked();
    void on_pushButton_z_reset_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
};

#endif // DIALOGSETRANGESETC_H
