#include "mainwindow.h"
#include <QApplication>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (!createConnection())
    {
        return 1;
    }

    MainWindow window;
    window.show();

    return a.exec();
}
