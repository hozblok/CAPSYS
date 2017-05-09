#include "projects_dialog.h"
#include "ui_projects_dialog.h"

ProjectsDialog::ProjectsDialog(QWidget *parent, QByteArray tTable) :
    QDialog(parent),
    ui(new Ui::projects_dialog),
    table(tTable)
{
    ui->setupUi(this);

    model = new QSqlTableModel(parent);
    model->setTable(table);
    model->select();

    ui->projs_tableView->setModel(model);
}

ProjectsDialog::~ProjectsDialog()
{
    delete ui;
}
QByteArray ProjectsDialog::getTable() const
{
    return table;
}

void ProjectsDialog::setTable(const QByteArray &value)
{
    table = value;
    model->setTable(table);
    model->select();
    ui->label->setText(QString(table));
}

void ProjectsDialog::slotSetTable(const QString &value)
{
    table = value.toUtf8();
    model->setTable(table);
    model->select();
    ui->label->setText(value);
}
