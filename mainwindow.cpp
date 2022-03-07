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
//    for(int i = 0; i < 30; i++)
//    {
//        connect(this->m_block[i], SIGNAL(show_tasks()), this->m_block[i], SLOT(slot_tasks));
//    }

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
    tmp_Label->setGeometry(400, 200 + this->DDL_number*200, 600, 200);
    tmp_Label->setParameters(400, 200 + this->DDL_number*200, 600, 200);
    tmp_Label->setStyleSheet("QLabel{border:2px solid rgb(0, 255, 255);}");
    isOccupied[this->DDL_number] = true;
    this->m_block[this->DDL_number++] = tmp_Label;
    time_t t = time(0);
    char tmp[64];
    strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A",localtime(&t) );
    QString now_time(tmp);
    //qDebug() << now_time;
    tmp_Label->setText(now_time);
    tmp_Label->show();

    connect(tmp_Label, SIGNAL(show_tasks()), tmp_Label, SLOT(slot_tasks()));

    //下面提供删除交互方式
    tmp_Label->Button_delete = new button_delete(tmp_Label);
    tmp_Label->Button_delete->setGeometry(0, 0, 150, 75);
    tmp_Label->Button_delete->setParameters(0, 0, 150, 75);
    tmp_Label->Button_delete->setText("delete");
    tmp_Label->Button_delete->show();
    tmp_Label->Button_delete->rank = DDL_number - 1;//记录这个ddl的序号，用于删除时恢复格式
    connect(tmp_Label->Button_delete, SIGNAL(delete_ddl(int)), this, SLOT(slot_delete(int)));

    //下面提供后继交互方式
    tmp_Label->Button_next = new button_next(tmp_Label);
    tmp_Label->Button_next->setGeometry(150, 0, 150, 75);
    tmp_Label->Button_next->setText("create succ");
    tmp_Label->Button_next->show();
    tmp_Label->Button_next->rank = tmp_Label->Button_delete->rank;
    connect(tmp_Label->Button_next, SIGNAL(next_ddl(int)), this, SLOT(slot_delete(int)));


}

void MainWindow::slot_delete(int rank)//用于维持正确格式并删除对应ddl
{
    if(DDL_number == 0)return;
    isOccupied[DDL_number - 1] = false;//clear out the position
    this->m_block[rank]->hide();//clear out the GUI of that ddl
    this->m_block[rank]->Button_delete->hide();
    for(int i = rank + 1; i < DDL_number; i++)
    {
        this->m_block[i - 1] = this->m_block[i];//依次前移
        this->m_block[i]->Button_delete->rank--;
        this->m_block[i]->Button_next->rank = this->m_block[i]->Button_delete->rank;
        this->m_block[i]->setGeometry(this->m_block[i]->x(), this->m_block[i]->y() - 200,
                                          this->m_block[i]->width(), this->m_block[i]->height());
        this->m_block[i]->Button_delete->setParameters(this->m_block[i]->x(), this->m_block[i]->y(),
                                                     200, 100);
        this->m_block[i]->Button_next->setParameters(this->m_block[i]->x() + 200, this->m_block[i]->y()
                                                     , 200, 100);
    }
    DDL_number--;
}

void MainWindow::slot_succ(int rank)
{
    create_ddl();
    this->m_block[DDL_number - 1]->m_ddl->SetNext(QString::number(rank, 10));
    qDebug() << this->m_block[DDL_number - 1]->m_ddl->Next;
}

//void MainWindow::slot_tasks()
//{
//    qDebug() << "sdfhjkdhk";
//}




