#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QShortcut>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include "linalg.h"
#include "eval_mpf.h"
#include "xrays.h"
#include "project.h"

#include "projects_dialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void addCapId_clicked();
    void addSourceId_clicked();
    void deleteCapId_clicked();
    void deleteSourceId_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_lineEdit_textEdited(const QString &arg1);

    void on_lineEdit_2_textEdited(const QString &arg1);

    void on_lineEdit_3_textEdited(const QString &arg1);

    void startSpread();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    ProjectsDialog *projs_dialog;
};

#endif // MAINWINDOW_H
