#include "mainwindow.h"
#include "ddl.h"
#include <QApplication>
#include <fstream>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
