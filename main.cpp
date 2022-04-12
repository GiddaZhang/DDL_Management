#include "mainwindow.h"
#include "ddl.h"
#include <QApplication>
#include <fstream>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    MainWindow w;
    w.show();
    return a.exec();
}
