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
//    QString d_1 = "2022-02-24 12:24:36", d_2 = "2019-03-31 12:24:38";
//    DDL D_1(d_1, "写Qt", "12:00:00", "Teamwork"), D_2(d_2, "电动力学第一章", "1:30:00", "homework");
//    qDebug() << D_2.GetUrgency();
//}
