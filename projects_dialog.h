#ifndef PROJECTS_DIALOG_H
#define PROJECTS_DIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include "settings.h"

namespace Ui {
class projects_dialog;
}

class ProjectsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectsDialog(QWidget *parent = 0, QByteArray tTable = "projs");
    ~ProjectsDialog();

    QByteArray getTable() const;
    void setTable(const QByteArray &value);

private:
    Ui::projects_dialog *ui;
    QSqlTableModel *model;
    QByteArray table;
    bool open_database();

public slots:
    void slotSetTable(const QString &value);
};

#endif // PROJECTS_DIALOG_H
