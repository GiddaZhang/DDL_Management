#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTime>
#include <ctime>
#include <string>
#include <QString>
#include <iostream>

// 构造函数
MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::MainWindow){

    // 设置并显示主窗口UI
    ui->setupUi(this);
    this->setGeometry(20, 40, 1600, 1080);
    this->show();
    this->setWindowTitle("DDL_management");

    // 设置并显示坐标轴UI
    AxisPainter *axis = new AxisPainter(this);
    axis->show();

    // 设置并显示左上角的新建DDL模块按键
    m_button = new button_new(this);
    m_button->setGeometry(30, 30, 80, 80);
    m_button->setStyleSheet("QLabel{border:2px solid rgb(0, 255, 0);}");
    m_button->setText("clickhere\nfor new ddl");
    m_button->show();

    // 将“新建”按钮的newddl信号与主窗口的create_ddl槽连接
    connect(this->m_button, SIGNAL(newddl()), this, SLOT(create_ddl()));

    //修改属性，设置为自定义菜单模式
        setContextMenuPolicy(Qt::CustomContextMenu);
        //关联信号和槽
        connect(this, &MainWindow::customContextMenuRequested, this, &MainWindow::showContextMenu );
}

// 析构函数
MainWindow::~MainWindow(){

    delete ui;
}

// 新建ddl快捷键(W)的设置
void MainWindow::keyPressEvent(QKeyEvent *event){

    if(event->key() == Qt::Key_W){
        create_ddl();
    }
}

// 槽：新建ddl模块
void MainWindow::create_ddl(){

    // 新建DDL指针并设置参数，维护DDL序列
    ddl_block *tmp_Label = new ddl_block(this);
    tmp_Label->setGeometry(400, 200 + this->DDL_number*200, 600, 200);
    tmp_Label->setParameters(400, 200 + this->DDL_number*200, 600, 200);
    tmp_Label->setStyleSheet("QLabel{border:2px solid rgb(0, 255, 255);}");
    isOccupied[this->DDL_number] = true;
    this->m_block[this->DDL_number++] = tmp_Label;

    // 在ddl模块上显示当前（新建时）时间
    time_t t = time(0);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y/%m/%d %X %A", localtime(&t));
    QString now_time(tmp);
    //qDebug() << now_time;
    tmp_Label->setText(now_time);
    tmp_Label->show();

    // 将当前ddl模块的show_tasks信号与其slot_tasks槽连接
    connect(tmp_Label, SIGNAL(show_tasks()), tmp_Label, SLOT(slot_tasks()));

    // 为ddl模块提供“删除”按钮
    tmp_Label->Button_delete = new button_delete(tmp_Label);
    tmp_Label->Button_delete->setGeometry(0, 0, 150, 75);
    tmp_Label->Button_delete->setParameters(0, 0, 150, 75);
    tmp_Label->Button_delete->setText("delete");
    tmp_Label->Button_delete->show();
    // 记录此ddl模块的秩，在删除时维护ddl序列
    tmp_Label->Button_delete->rank = DDL_number - 1;
    // 将“删除”按钮的delete_ddl信号与主窗口的slot_delete槽连接
    connect(tmp_Label->Button_delete, SIGNAL(delete_ddl(int)), this, SLOT(slot_delete(int)));

    // 为ddl模块提供“后继”按钮，操作同上
    tmp_Label->Button_next = new button_next(tmp_Label);
    tmp_Label->Button_next->setGeometry(150, 0, 150, 75);
    tmp_Label->Button_next->setText("create succ");
    tmp_Label->Button_next->show();
    tmp_Label->Button_next->rank = tmp_Label->Button_delete->rank;
    connect(tmp_Label->Button_next, SIGNAL(next_ddl(int)), this, SLOT(slot_delete(int)));
}

//按秩删除ddl并维护ddl序列
void MainWindow::slot_delete(int rank){

    // 判断early exit条件
    if(DDL_number == 0)return;
    isOccupied[DDL_number - 1] = false;//clear out the position
    this->m_block[rank]->hide();//clear out the GUI of that ddl
    this->m_block[rank]->Button_delete->hide();
    for(int i = rank + 1; i < DDL_number; i++){
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

// 为当前DDl设置后继
void MainWindow::slot_succ(int rank){
    create_ddl();
    this->m_block[DDL_number - 1]->m_ddl->SetNext(QString::number(rank, 10));
    qDebug() << this->m_block[DDL_number - 1]->m_ddl->GetNext();
}

void MainWindow::paintEvent(QPaintEvent *event){
    QPainter *painter = new QPainter(this);
    QPen pen;
    pen.setColor(Qt::black);
    painter->setPen(pen);
    QPoint axisStartPoint;
    QPoint axisXEndPoint; // x 轴终点
    QPoint axisYEndPoint; // y 轴终点

    axisStartPoint.setX(30);
    axisStartPoint.setY(1030);

    axisXEndPoint.setX(1600);
    axisXEndPoint.setY(1030);

    axisYEndPoint.setX(30);
    axisYEndPoint.setY(30);

    painter->drawLine(axisStartPoint, axisXEndPoint);
    painter->drawLine(axisStartPoint, axisYEndPoint);
}

//void MainWindow::slot_tasks()
//{
//    qDebug() << "sdfhjkdhk";
//}

// 问题：DDL_number在构造函数里没有初始化为0
// 问题：析构函数没有处理delete
// 问题：在成员变量处赋值，Is_occupied在构造函数里没有初始化为false
// 问题：slot_delete没有判断rank处本来就无ddl的情况
// 问题：next_ddl信号错误连接到slot_delete上
// 问题：slot_delete没有处理delete
// 问题：slot_delete没有处理next_button的hide，但是实际上能够hide
// 问题：是否能够将ddl_block与相应几个按钮组合
// 问题：paintEvent函数中重复写入QPainter中的代码
// 问题：slot_delete需要删除底层all_ddl中的相应指针。需要手动调用all_ddl的erase函数以避免移位操作
// ... 另slot_delete需要析构ddl_block，这会重复析构其中的m_ddl，可能需要智能指针
