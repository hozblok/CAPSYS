#include "g_mainwindow.h"
#include "ui_g_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mglWindowCreated(false)
{
    ui->setupUi(this);
    graph = new Graph(this);

    connect(ui->spinBoxCapId, SIGNAL(valueChanged(int)), graph, SLOT(slotSetCapId(int)));
    connect(ui->spinBoxSourceId, SIGNAL(valueChanged(int)), graph, SLOT(slotSetSourceId(int)));
    connect(ui->spinBox_count_rays, SIGNAL(valueChanged(int)), graph, SLOT(slotSetCountRays(int)));
}

MainWindow::~MainWindow()
{
    if(mglWindowCreated)
    {
        delete mglWindow;
    }
    delete graph;
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    qDebug() << "AAAA" << mglWindowCreated;
    if(mglWindowCreated)
    {
        mglWindow->Update();
    }
    else
    {
        mglWindow = new mglQT(graph, "MathGL Graph");
    }
    if(mglWindow->Run() == 0)
    {
        mglWindowCreated = true;
    }
    qDebug() << "CCCC" << mglWindowCreated;
}
