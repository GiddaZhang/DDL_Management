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

//void test()
//{
//    DDL::LoadFromFile();
//    DDL::SaveToFile();
//    shared_ptr<DDL> a = DDL::GetDDLPtr("Homework");
//    a->OpenAllFile();
//}

