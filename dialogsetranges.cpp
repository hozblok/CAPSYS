#include "dialogsetranges.h"
#include "ui_dialogsetranges.h"

DialogSetRanges::DialogSetRanges(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetRanges)
{
    ui->setupUi(this);

    QObject::connect(ui->doubleSpinBox_x_,  SIGNAL(valueChanged(double)), this, SLOT(slotSetXLower(double)));
    QObject::connect(ui->doubleSpinBox_y_,  SIGNAL(valueChanged(double)), this, SLOT(slotSetYLower(double)));
    QObject::connect(ui->doubleSpinBox_z_,  SIGNAL(valueChanged(double)), this, SLOT(slotSetZLower(double)));
    QObject::connect(ui->doubleSpinBox_x__, SIGNAL(valueChanged(double)), this, SLOT(slotSetXUpper(double)));
    QObject::connect(ui->doubleSpinBox_y__, SIGNAL(valueChanged(double)), this, SLOT(slotSetYUpper(double)));
    QObject::connect(ui->doubleSpinBox_z__, SIGNAL(valueChanged(double)), this, SLOT(slotSetZUpper(double)));
    QObject::connect(ui->doubleSpinBox_rx,  SIGNAL(valueChanged(double)), this, SLOT(slotSetRotateX(double)));
    QObject::connect(ui->doubleSpinBox_ry,  SIGNAL(valueChanged(double)), this, SLOT(slotSetRotateY(double)));
    QObject::connect(ui->doubleSpinBox_rz,  SIGNAL(valueChanged(double)), this, SLOT(slotSetRotateZ(double)));
}

DialogSetRanges::~DialogSetRanges()
{
    delete ui;
}

void DialogSetRanges::setParams(const double _xLowerLimit, const double _xUpperLimit,
                                const double _yLowerLimit, const double _yUpperLimit,
                                const double _zLowerLimit, const double _zUpperLimit,
                                const double _rotateX, const double _rotateY, const double _rotateZ)
{
    ui->doubleSpinBox_x_->setValue(_xLowerLimit);
    ui->doubleSpinBox_y_->setValue(_yLowerLimit);
    ui->doubleSpinBox_z_->setValue(_zLowerLimit);
    ui->doubleSpinBox_x__->setValue(_xUpperLimit);
    ui->doubleSpinBox_y__->setValue(_yUpperLimit);
    ui->doubleSpinBox_z__->setValue(_zUpperLimit);
    ui->doubleSpinBox_rx->setValue(_rotateX);
    ui->doubleSpinBox_ry->setValue(_rotateY);
    ui->doubleSpinBox_rz->setValue(_rotateZ);
}

void DialogSetRanges::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DialogSetRanges::on_buttonBox_accepted()
{
    this->reject();
}

void DialogSetRanges::on_pushButton_anglreset_clicked()
{
    ui->doubleSpinBox_rx->setValue(60.);
    ui->doubleSpinBox_ry->setValue(0.);
    ui->doubleSpinBox_rz->setValue(40.);
}

void DialogSetRanges::on_pushButton_z_reset_clicked()
{
    ui->doubleSpinBox_z_->setValue(0.);
}

void DialogSetRanges::on_pushButton_2_clicked()
{
    ui->doubleSpinBox_x_->setValue(ui->doubleSpinBox_x_->value() / 2);
    ui->doubleSpinBox_x__->setValue(ui->doubleSpinBox_x__->value() / 2);
    ui->doubleSpinBox_y_->setValue(ui->doubleSpinBox_y_->value() / 2);
    ui->doubleSpinBox_y__->setValue(ui->doubleSpinBox_y__->value() / 2);
}

void DialogSetRanges::on_pushButton_clicked()
{
    ui->doubleSpinBox_x_->setValue(ui->doubleSpinBox_x_->value() * 2);
    ui->doubleSpinBox_x__->setValue(ui->doubleSpinBox_x__->value() * 2);
    ui->doubleSpinBox_y_->setValue(ui->doubleSpinBox_y_->value() * 2);
    ui->doubleSpinBox_y__->setValue(ui->doubleSpinBox_y__->value() * 2);
}
