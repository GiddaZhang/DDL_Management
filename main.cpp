#include "mainwindow.h"
#include "ddl.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//    void test();
//    test();
    return a.exec();
}

//void test()
//{
//    QString d_1 = "2019-03-31 12:24:36", d_2 = "2020-03-31 12:24:36";
//    DDL D_1(d_1, "写Qt", "Teamwork"), D_2(d_2, "电动力学第一章", "homework");
//    qDebug() << (D_1 < D_2);
//}
