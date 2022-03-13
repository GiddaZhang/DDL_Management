#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTime>
#include <ctime>
#include <string>
#include <QString>
#include <iostream>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QDialog>
#include <QInputDialog>

// 构造函数
MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::MainWindow){
    //初始化DDL_number
    this->DDL_number = 0;
    this->DDL_lines_number = 0;

    // 设置并显示主窗口UI
    ui->setupUi(this);
    this->setGeometry(20, 40, 1920, 1080);
    AxisPainter *axis = new AxisPainter(this);
    axis->show();
    this->show();
    this->setWindowTitle("DDL_management");

    // 设置并显示坐标轴UI

    // 设置并显示左上角的新建DDL模块按键
    m_button = new button_new(this);
    m_button->setGeometry(0, 0, 100, 100);
    m_button->setStyleSheet("QLabel{border:2px solid rgb(0, 255, 0);}");
    m_button->setText("clickhere\nfor new ddl");
    m_button->show();

    // 将“新建”按钮的newddl信号与主窗口的create_ddl槽连接
    connect(this->m_button, SIGNAL(newddl()), this, SLOT(create_ddl()));

    //修改属性，设置为自定义菜单模式
    setContextMenuPolicy(Qt::CustomContextMenu);
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
     //新建DDL指针并设置参数，维护DDL序列,并且初始化ddl
    ddl_block *tmp_Label = new ddl_block(this);
    tmp_Label->setStyleSheet("QLabel{border:2px solid rgb(0, 255, 255);}");
    isOccupied[this->DDL_number] = true;
    this->m_block[this->DDL_number] = tmp_Label;
    tmp_Label->rank = this->DDL_number;
    tmp_Label->line_rank = DDL_lines_number;
    this->number_each_line[tmp_Label->line_rank]++;
    DDL_number++;
    DDL_lines_number++;
    //初始化ddl


    //输入当前时间，格式为yyyy-MM-dd hh:mm:ss，存储在QString变量里
    QInputDialog type_in_commence(tmp_Label);
    QString comm_time = type_in_commence.getText(tmp_Label, "comm_time", "please type in commence time", QLineEdit::Normal);
    QInputDialog type_in_due(tmp_Label);
    QString due_time = type_in_due.getText(tmp_Label, "due_time", "please type in due time", QLineEdit::Normal);

     //输入并在ddl块上显示当前时间
      QInputDialog type_in_name(tmp_Label);
      QString tmp_name = type_in_name.getText(tmp_Label, "name", "please type in ddl name", QLineEdit::Normal);
      tmp_Label->m_ddl->SetName(tmp_name);
      tmp_Label->setText(tmp_name);

    //tmp_Label->setText(QString::number(tmp_Label->rank, 10) + QString::number(tmp_Label->line_rank, 10));


    //测试版
//    QString comm_time = "2022-03-14 00:00:00";
//    QString due_time = "2022-03-15 00:00:00";


//    //初始化ddl
    tmp_Label->m_ddl = new DDL("UNKNOWN", comm_time,
                               due_time, "PLAIN", "NULL",
                               0, 0.0, "PREV","NEXT");
    //获得ddl持续时间，决定其在界面上的长度和位置
    QDateTime begin_time = QDateTime::fromString(comm_time, "yyyy-MM-dd hh:mm:ss");
    QDateTime end_time = QDateTime::fromString(due_time, "yyyy-MM-dd hh:mm:ss");
    QDateTime curr_time = QDateTime::currentDateTime();

    //qDebug() << begin_time.daysTo(end_time);//获取ddl的长度（日）
    tmp_Label->setGeometry(200 + tmp_Label->line_rank * 200, 1080 - 200 * curr_time.daysTo(end_time), 200, begin_time.daysTo(end_time) * 200);
    //tmp_Label->setGeometry(400, 400, 400, 400);

    // 将当前ddl模块的show_tasks信号与其slot_tasks槽连接
    connect(tmp_Label, SIGNAL(show_tasks()), tmp_Label, SLOT(slot_tasks()));

    // 为ddl模块提供“删除”按钮
    tmp_Label->Button_delete = new button_delete(tmp_Label);
//    tmp_Label->Button_delete->setGeometry(0, 0, 150, 75);
//    tmp_Label->Button_delete->setParameters(0, 0, 150, 75);
    tmp_Label->Button_delete->setText("delete");
    //tmp_Label->Button_delete->show();
    // 将“删除”按钮的delete_ddl信号与主窗口的slot_delete槽连接
    connect(tmp_Label->Button_delete, SIGNAL(delete_ddl(int)), this, SLOT(slot_delete(int)));
    connect(tmp_Label, SIGNAL(getInt(int)), this, SLOT(slot_delete(int)));
    connect(tmp_Label, SIGNAL(getInt_succ(int)), this, SLOT(slot_succ(int)));
    connect(tmp_Label, SIGNAL(getInt_prev(int)), this, SLOT(slot_prev(int)));



    // 为ddl模块提供“后继”按钮，操作同上
    tmp_Label->Button_next = new button_next(tmp_Label);
//    tmp_Label->Button_next->setGeometry(150, 0, 150, 75);
    tmp_Label->Button_next->setText("succ");
    //tmp_Label->Button_next->show();

    // 为ddl模块提供“前驱”按钮，操作同上
    tmp_Label->Button_prev = new button_prev(tmp_Label);
    //tmp_Label->Button_prev->setGeometry(0, 150, 150, 75);
    tmp_Label->Button_prev->setText("prev");
    //tmp_Label->Button_prev->show();

    // 记录此ddl模块的秩，在删除时维护ddl序列
    tmp_Label->Button_delete->rank = DDL_number - 1;
    tmp_Label->Button_next->rank = DDL_number - 1;
    tmp_Label->Button_prev->rank = DDL_number - 1;
    tmp_Label->rank = DDL_number - 1;

    connect(tmp_Label->Button_next, SIGNAL(next_ddl(int)), this, SLOT(slot_succ(int)));
    connect(tmp_Label->Button_prev, SIGNAL(prev_ddl(int)), this, SLOT(slot_prev(int)));
    tmp_Label->show();

    // 下面开始设置ddl_block出现的菜单
    tmp_Label->setContextMenuPolicy(Qt::CustomContextMenu);
    //鼠标右键点击控件时会发送一个void QWidget::customContextMenuRequested(const QPoint &pos)信号
    //给信号设置相应的槽函数
    connect(tmp_Label, &QLabel::customContextMenuRequested, [=](const QPoint &pos)
    {
        tmp_Label->m_pMenu->exec(QCursor::pos());
    });
    //qDebug() << tmp_Label->line_rank;
    //qDebug() << tmp_Label->x() << tmp_Label->y() << tmp_Label->width() << tmp_Label->height();
    tmp_Label->Button_prev->hide();
    tmp_Label->Button_next->hide();
    tmp_Label->Button_delete->hide();
}

//按秩删除ddl并维护ddl序列
void MainWindow::slot_delete(int rank){

    // 判断early exit条件
    if(DDL_number == 0)return;
    isOccupied[DDL_number - 1] = false;//clear out the position
    this->m_block[rank]->hide();//clear out the GUI of that ddl
    // 给前驱后继擦屁股
    if(m_block[rank]->m_ddl->GetPrev() != "PREV")
    {
        m_block[rank]->m_ddl->SetPrev("PREV");
    }
    if(m_block[rank]->m_ddl->GetNext() != "NEXT")
    {
        m_block[rank]->m_ddl->SetNext("NEXT");
    }
    this->m_block[rank]->Button_delete->hide();

    int deleted_linerank = this->m_block[rank]->line_rank;//记录被灭绝的line's rank

    for(int i = rank + 1; i < DDL_number; i++)
    {
        this->m_block[i - 1] = this->m_block[i];//依次前移
        this->m_block[i - 1]->rank--;
//        qDebug() << m_block[i - 1]->rank;
        this->m_block[i - 1]->setText(QString::number(this->m_block[i - 1]->rank));
        this->m_block[i - 1]->Button_delete->rank--;
        this->m_block[i - 1]->Button_next->rank = this->m_block[i]->Button_delete->rank;
    }
    //调整位置
    this->number_each_line[deleted_linerank]--;

    //如果这个ddl删除导致了少了一条line
    if(this->number_each_line[deleted_linerank] == 0)
    {
        for(int i = deleted_linerank; i < DDL_lines_number - 1; i++)//重新计数
            number_each_line[i] = 0;
        for(int i = 0; i < DDL_number - 1; i++)//对于每一个ddl
        {
            if(this->m_block[i]->line_rank > deleted_linerank)//如果他的linerank大于被删掉的
            {
                this->m_block[i]->line_rank--;
                number_each_line[this->m_block[i]->line_rank]++;
            }
            this->m_block[i]->setGeometry(200 + this->m_block[i]->line_rank * 200,
                                          this->m_block[i]->y(), 200, this->m_block[i]->height());
        }
        this->number_each_line[DDL_lines_number] = 0;
        this->DDL_lines_number--;//少了一条线

    }
    //如果这个ddl删除没有导致少了一条line,不需要更多改动了
    DDL_number--;
}

// 为当前DDl设置后继
void MainWindow::slot_succ(int rank){
    ddl_block *tmp_Label = new ddl_block(this);
    tmp_Label->setStyleSheet("QLabel{border:2px solid rgb(0, 255, 255);}");
    isOccupied[this->DDL_number] = true;
    this->m_block[this->DDL_number] = tmp_Label;
    tmp_Label->rank = this->DDL_number;
    tmp_Label->line_rank = m_block[rank]->line_rank;//后继应该与被后继的在同一line中
    this->number_each_line[tmp_Label->line_rank]++;
    DDL_number++;

    //输入当前时间，格式为yyyy-MM-dd hh:mm:ss，存储在QString变量里
    QInputDialog type_in_commence(tmp_Label);
    QString comm_time = type_in_commence.getText(tmp_Label, "comm_time", "please type in commence time", QLineEdit::Normal);
    QInputDialog type_in_due(tmp_Label);
    QString due_time = type_in_due.getText(tmp_Label, "due_time", "please type in due time", QLineEdit::Normal);

    QInputDialog type_in_name(tmp_Label);
    QString tmp_name = type_in_name.getText(tmp_Label, "name", "please type in ddl name", QLineEdit::Normal);
    tmp_Label->m_ddl->SetName(tmp_name);
    tmp_Label->setText(tmp_name);

    //测试版
//    QString comm_time = "2022-03-15 00:00:00";
//    QString due_time = "2022-03-16 00:00:00";
    QDateTime begin_time = QDateTime::fromString(comm_time, "yyyy-MM-dd hh:mm:ss");
    QDateTime end_time = QDateTime::fromString(due_time, "yyyy-MM-dd hh:mm:ss");
    QDateTime curr_time = QDateTime::currentDateTime();

    m_block[rank]->m_ddl->SetNext(QString::number(DDL_number - 1, 10));//原来的ddl的后继的序号是新的ddl的序号
    tmp_Label->m_ddl->SetPrev(QString::number(rank, 10));//新的ddl的前驱的序号是原来的ddl

    // 将当前ddl模块的show_tasks信号与其slot_tasks槽连接
    connect(tmp_Label, SIGNAL(show_tasks()), tmp_Label, SLOT(slot_tasks()));

    // 为ddl模块提供“删除”按钮
    tmp_Label->Button_delete = new button_delete(tmp_Label);
//    tmp_Label->Button_delete->setGeometry(0, 0, 150, 75);
//    tmp_Label->Button_delete->setParameters(0, 0, 150, 75);
    tmp_Label->Button_delete->setText("delete");
    //tmp_Label->Button_delete->show();

    // 将“删除”按钮的delete_ddl信号与主窗口的slot_delete槽连接
    connect(tmp_Label->Button_delete, SIGNAL(delete_ddl(int)), this, SLOT(slot_delete(int)));

    // 为ddl模块提供“后继”按钮，操作同上
    tmp_Label->Button_next = new button_next(tmp_Label);
//    tmp_Label->Button_next->setGeometry(150, 0, 150, 75);
    tmp_Label->Button_next->setText("create succ");
    //tmp_Label->Button_next->show();

    // 为ddl模块提供“后继”按钮，操作同上
    tmp_Label->Button_prev = new button_prev(tmp_Label);
    //tmp_Label->Button_prev->setGeometry(0, 150, 150, 75);
    tmp_Label->Button_prev->setText("create prev");
    //tmp_Label->Button_prev->show();

    // 记录此ddl模块的秩，在删除时维护ddl序列
    tmp_Label->Button_delete->rank = DDL_number - 1;
    tmp_Label->Button_next->rank = DDL_number - 1;
    tmp_Label->rank = DDL_number - 1;
    //qDebug() << m_block[rank]->rank << tmp_Label->rank;
    //connect(tmp_Label->Button_next, SIGNAL(next_ddl(int)), this, SLOT(slot_succ(int)));
    tmp_Label->setGeometry(m_block[rank]->x(), 1080 - 200 * curr_time.daysTo(end_time), 200, begin_time.daysTo(end_time) * 200);
    tmp_Label->show();
    //tmp_Label->setText(QString::number(tmp_Label->rank, 10) + QString::number(tmp_Label->line_rank, 10));

    // 下面开始设置ddl_block出现的菜单
    tmp_Label->setContextMenuPolicy(Qt::CustomContextMenu);
    //鼠标右键点击控件时会发送一个void QWidget::customContextMenuRequested(const QPoint &pos)信号
    //给信号设置相应的槽函数
    connect(tmp_Label, &QLabel::customContextMenuRequested, [=](const QPoint &pos)
    {
        tmp_Label->m_pMenu->exec(QCursor::pos());
    });

    //获得ddl持续时间，决定其在界面上的长度和位置
    tmp_Label->Button_prev->hide();
    tmp_Label->Button_next->hide();
    tmp_Label->Button_delete->hide();
    connect(tmp_Label, SIGNAL(getInt(int)), this, SLOT(slot_delete(int)));
    connect(tmp_Label, SIGNAL(getInt_succ(int)), this, SLOT(slot_succ(int)));
    connect(tmp_Label, SIGNAL(getInt_prev(int)), this, SLOT(slot_prev(int)));
}

void MainWindow::slot_prev(int rank)
{
    ddl_block *tmp_Label = new ddl_block(this);
    tmp_Label->setStyleSheet("QLabel{border:2px solid rgb(0, 255, 255);}");
    isOccupied[this->DDL_number] = true;
    this->m_block[this->DDL_number] = tmp_Label;
    tmp_Label->rank = this->DDL_number;
    tmp_Label->line_rank = m_block[rank]->line_rank;//后继应该与被后继的在同一line中
    this->number_each_line[tmp_Label->line_rank]++;
    DDL_number++;

    //输入当前时间，格式为yyyy-MM-dd hh:mm:ss，存储在QString变量里
    QInputDialog type_in_commence(tmp_Label);
    QString comm_time = type_in_commence.getText(tmp_Label, "comm_time", "please type in commence time", QLineEdit::Normal);
    QInputDialog type_in_due(tmp_Label);
    QString due_time = type_in_due.getText(tmp_Label, "due_time", "please type in due time", QLineEdit::Normal);

    //测试版
//    QString comm_time = "2022-03-13 00:00:00";
//    QString due_time = "2022-03-15 00:00:00";
    QDateTime begin_time = QDateTime::fromString(comm_time, "yyyy-MM-dd hh:mm:ss");
    QDateTime end_time = QDateTime::fromString(due_time, "yyyy-MM-dd hh:mm:ss");
    QDateTime curr_time = QDateTime::currentDateTime();

    QInputDialog type_in_name(tmp_Label);
    QString tmp_name = type_in_name.getText(tmp_Label, "name", "please type in ddl name", QLineEdit::Normal);
    tmp_Label->m_ddl->SetName(tmp_name);
    tmp_Label->setText(tmp_name);

    m_block[rank]->m_ddl->SetPrev(QString::number(DDL_number - 1, 10));//原来的ddl的后继的序号是新的ddl的序号
    tmp_Label->m_ddl->SetNext(QString::number(rank, 10));//新的ddl的前驱的序号是原来的ddl

    // 将当前ddl模块的show_tasks信号与其slot_tasks槽连接
    connect(tmp_Label, SIGNAL(show_tasks()), tmp_Label, SLOT(slot_tasks()));

    // 为ddl模块提供“删除”按钮
    tmp_Label->Button_delete = new button_delete(tmp_Label);
    tmp_Label->Button_delete->setGeometry(0, 0, 150, 75);
    tmp_Label->Button_delete->setParameters(0, 0, 150, 75);
    tmp_Label->Button_delete->setText("delete");
    tmp_Label->Button_delete->show();

    // 将“删除”按钮的delete_ddl信号与主窗口的slot_delete槽连接
    connect(tmp_Label->Button_delete, SIGNAL(delete_ddl(int)), this, SLOT(slot_delete(int)));

    // 为ddl模块提供“后继”按钮，操作同上
    tmp_Label->Button_prev = new button_prev(tmp_Label);
    tmp_Label->Button_prev->setGeometry(0, 150, 150, 75);
    tmp_Label->Button_prev->setText("create prev");
    tmp_Label->Button_prev->show();

    // 为ddl模块提供“后继”按钮，操作同上
    tmp_Label->Button_next = new button_next(tmp_Label);
    tmp_Label->Button_next->setGeometry(150, 0, 150, 75);
    tmp_Label->Button_next->setText("create succ");
    tmp_Label->Button_next->show();

    // 记录此ddl模块的秩，在删除时维护ddl序列
    tmp_Label->Button_delete->rank = DDL_number - 1;
    tmp_Label->Button_prev->rank = DDL_number - 1;
    tmp_Label->rank = DDL_number - 1;
    //qDebug() << m_block[rank]->rank << tmp_Label->rank;
    //connect(tmp_Label->Button_next, SIGNAL(next_ddl(int)), this, SLOT(slot_succ(int)));
    tmp_Label->setGeometry(m_block[rank]->x(), 1080 - 200 * curr_time.daysTo(end_time), 200, begin_time.daysTo(end_time) * 200);
    tmp_Label->show();
    //tmp_Label->setText(QString::number(tmp_Label->rank, 10) + QString::number(tmp_Label->line_rank, 10));

    // 下面开始设置ddl_block出现的菜单
    tmp_Label->setContextMenuPolicy(Qt::CustomContextMenu);
    //鼠标右键点击控件时会发送一个void QWidget::customContextMenuRequested(const QPoint &pos)信号
    //给信号设置相应的槽函数
    connect(tmp_Label, &QLabel::customContextMenuRequested, [=](const QPoint &pos)
    {
        tmp_Label->m_pMenu->exec(QCursor::pos());
    });

    //获得ddl持续时间，决定其在界面上的长度和位置
    tmp_Label->Button_prev->hide();
    tmp_Label->Button_next->hide();
    tmp_Label->Button_delete->hide();
    connect(tmp_Label, SIGNAL(getInt(int)), this, SLOT(slot_delete(int)));
    connect(tmp_Label, SIGNAL(getInt_succ(int)), this, SLOT(slot_succ(int)));
    connect(tmp_Label, SIGNAL(getInt_prev(int)), this, SLOT(slot_prev(int)));
}

//void MainWindow::paintEvent(QPaintEvent *event){
//    QPainter *painter = new QPainter(this);
//    QPen pen;
//    pen.setColor(Qt::black);
//    painter->setPen(pen);
//    QPoint axisStartPoint;
//    QPoint axisXEndPoint; // x 轴终点
//    QPoint axisYEndPoint; // y 轴终点

//    axisStartPoint.setX(30);
//    axisStartPoint.setY(1600);

//    axisXEndPoint.setX(1600);
//    axisXEndPoint.setY(1600);

//    axisYEndPoint.setX(30);
//    axisYEndPoint.setY(30);

//    painter->drawLine(axisStartPoint, axisXEndPoint);
//    painter->drawLine(axisStartPoint, axisYEndPoint);
//}

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
// 问题：paintEvent函数中重复写入QPainter中的代码，是否考虑加入一个AxisPainter成员
// 问题：slot_delete需要删除底层all_ddl中的相应指针。需要手动调用all_ddl的erase函数以避免移位操作
// ... 另slot_delete需要析构ddl_block，这会重复析构其中的m_ddl，可能需要智能指针

