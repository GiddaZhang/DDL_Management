#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTime>
#include <ctime>
#include <string>
#include <QString>
#include <QScrollArea>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 主窗口设置
    this->setGeometry(20, 40, 1100, 760);
    this->show();
    this->setWindowTitle("DDL_Management");

    // 滚动区域设置
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setGeometry(20, 200, 1050, 760);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);   // 让滚动条可见

    m_button = new button_new(this);
    m_button->setGeometry(20, 40, 200, 200);
    m_button->setStyleSheet("QLabel{border:2px solid rgb(0, 255, 0);}");
    m_button->setText("clickhere\nfor new ddl");
    m_button->show();
    connect(this->m_button, SIGNAL(newddl()), this, SLOT(create_ddl()));

    QWidget *w = new QWidget(s);
    //设置滚动区域的窗体
    scrollArea->setWidget(w);
    w->setGeometry(0, 0, 3000, 3000);
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
    //qDebug() << "shit";
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

    //下面提供删除交互方式
    tmp_Label->Button_delete = new button_delete(tmp_Label);
    tmp_Label->Button_delete->setParameters(tmp_Label->m_x, tmp_Label->m_y, 200, 100);
    tmp_Label->Button_delete->setText("delete");
    tmp_Label->Button_delete->show();
    tmp_Label->Button_delete->rank = DDL_number - 1;//记录这个ddl的序号，用于删除时恢复格式
    //tmp_Label->Button_delete->setText(QString::number(tmp_Label->Button_delete->rank, 10));
    connect(tmp_Label->Button_delete, SIGNAL(delete_ddl(int)), this, SLOT(slot_delete(int)));
}

void MainWindow::slot_delete(int rank)//用于维持正确格式并删除对应ddl
{
    if(DDL_number == 0)return;
    //qDebug() << rank;
    isOccupied[DDL_number - 1] = false;//clear out the position
    this->m_block[rank]->hide();//clear out the GUI of that ddl
    this->m_block[rank]->Button_delete->hide();
    for(int i = rank + 1; i < DDL_number; i++)
    {
        //qDebug() << i;
        this->m_block[i - 1] = this->m_block[i];//依次前移
        this->m_block[i]->Button_delete->rank--;

        //this->m_block[i]->Button_delete->setText(QString::number(this->m_block[i]->Button_delete->rank, 10));
        this->m_block[i]->setGeometry(this->m_block[i]->x(), this->m_block[i]->y() - 200,
                                          this->m_block[i]->width(), this->m_block[i]->height());
        this->m_block[i]->Button_delete->setParameters(this->m_block[i]->x(), this->m_block[i]->y(),
                                                     200, 100);
        //qDebug() << this->m_block[i]->y() << this->m_block[i]->Button_delete->y();
    }
//    for(int i = 0; i < DDL_number; i++)
//    {
//        qDebug() << DDL_number;
//        qDebug() << isOccupied[i];
//    }
    //qDebug() << this->m_block[rank]->text();
    //this->m_block[DDL_number - 1] = nullptr;
    DDL_number--;
}




