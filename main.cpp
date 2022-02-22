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
//    Date d_1(0,0,0,1,1,2002), d_2(0,0,0,1,1,2003);
//    DDL D_1(d_1, "Do homework"), D_2(d_2, "Exercise");
//    qDebug() << (D_1 < D_2);
//}
