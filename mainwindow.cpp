#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTime>
#include <ctime>
#include <string>
#include <QString>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setGeometry(0, 0, 1600, 1600);
    this->show();
    this->setWindowTitle("DDL_management");
    m_button = new QLabel(this);
    m_button->setGeometry(0, 0, 400, 400);
    m_button->setStyleSheet("QLabel{border:2px solid rgb(0, 255, 0);}");
    m_button->setText("clickhere\nfor new ddl");
    m_button->show();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W)
    {
        create_ddl();
    }
}

void MainWindow::create_ddl()
{
    ddl_block *tmp_Label = new ddl_block(this);
    tmp_Label->setGeometry(400, 400, this->DDL_number*200, this->DDL_number*200);
    this->DDL_number++;
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int Year = ltm->tm_year;
    int Month = ltm->tm_mon;
    int Date = ltm->tm_mday;
    int Hour = ltm->tm_hour;
    int Minute = ltm->tm_min;
    int Second = ltm->tm_sec;
    QString ddl_time = to_string(Year) + " " + Month + " " + Date + " " + Hour + " " + Minute + " " +  Second;
    DDL *tmp_DDL = new DDL(0, false, false,
                           QString time, QString des, QString dur,
                           DDL* prev, DDL* next)

}
