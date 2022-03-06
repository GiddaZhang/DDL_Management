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

//    void test();
//    test();
    return a.exec();
}

void test()
{
//    QString l = "\\", path_1 = R"(D:\Desktop)", path_2 = "D:\\Desktop";
//    if(!path_2.contains(l, Qt::CaseInsensitive)) {
//        qDebug() << 1;
//    }
//    else
//        qDebug() << 2;

    DDL::LoadFromFile();
    DDL::SaveToFile();

//    auto it = DDL::m_allDDL.begin() + 1;
//    (*it)->OpenAllFile();
}

