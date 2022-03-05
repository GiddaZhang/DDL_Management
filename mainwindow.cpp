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
    m_button = new button_new(this);
    m_button->setGeometry(0, 0, 400, 400);
    m_button->setStyleSheet("QLabel{border:2px solid rgb(0, 255, 0);}");
    m_button->setText("clickhere\nfor new ddl");
    m_button->show();
    connect(this->m_button, SIGNAL(newddl()), this, SLOT(create_ddl()));

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
    qDebug() << "shit";
    ddl_block *tmp_Label = new ddl_block(this);
    tmp_Label->setGeometry(400, 200 + this->DDL_number*200, 600, 200);
    tmp_Label->setParameters(400, 200 + this->DDL_number*200, 600, 200);
    tmp_Label->setStyleSheet("QLabel{border:2px solid rgb(0, 255, 255);}");
    isOccupied[this->DDL_number] = true;
    this->m_block[this->DDL_number++] = tmp_Label;
    time_t t = time(0);
    char tmp[64];
    strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A",localtime(&t) );
    QString now_time(tmp);
    qDebug() << now_time;
    tmp_Label->setText(now_time);
    tmp_Label->show();

    //下面提供删除交互方式
    tmp_Label->Button_delete = new button_delete(tmp_Label);
    tmp_Label->setParameters(tmp_Label->m_x, tmp_Label->m_y, 200, 100);
    tmp_Label->Button_delete->setText("delete");
    tmp_Label->Button_delete->show();
    tmp_Label->Button_delete->rank = DDL_number - 1;//记录这个ddl的序号，用于删除时恢复格式
    connect(tmp_Label->Button_delete, SIGNAL(delete_ddl(int)), this, SLOT(slot_delete(int)));
}

void MainWindow::slot_delete(int rank)//用于维持正确格式并删除对应ddl
{
    //qDebug() << rank;
    isOccupied[rank] = false;//clear out the position
    for(int i = rank + 1; i < DDL_number; i++)
    {
        this->m_block[i]->setGeometry(this->x(), this->y() - 200, this->width(), this->height());
    }
}




