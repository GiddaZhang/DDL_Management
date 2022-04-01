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
#include <QDateTimeEdit>
#include <QGridLayout>

// 构造函数
MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent), ui(new Ui::MainWindow){
    //初始化DDL_number
    //this->DDL_number = 0;
    this->DDL_lines_number = 0;

    // 设置并显示主窗口UI
    ui->setupUi(this);
    this->setGeometry(20, 20, 1920, 1080);
//    AxisPainter *axis = new AxisPainter(this);
//    axis->show();
    this->setWindowTitle("DDL_MANAGEMENT");

    // 初始化菜单栏和菜单项
    this->m_menuBar = this->menuBar();
    m_menuBar->setMinimumHeight(30);
    this->m_Menu = this->m_menuBar->addMenu("开始");
    this->m_createDDLAction = new QAction("新建DDL", this);
    this->m_Menu->addAction(m_createDDLAction);        // 将菜单项添加到子菜单
    // 添加菜单响应函数
    connect(m_createDDLAction, &QAction::triggered, this, &MainWindow::create_ddl);

    // 初始化滚动条
    this->m_scrollArea = new QScrollArea(this);
    m_scrollArea->setBackgroundRole(QPalette::Light);                   // 背景色
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);    // 只让竖直滚动
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setGeometry(0, 30, 1920, 1080);

    // 初始化滚动区域
    this->m_scrollWidget = new QWidget(this);
    this->m_scrollWidget->setGeometry(0, 0, 1920, 3000);
    m_scrollArea->setWidget(this->m_scrollWidget);
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

void MainWindow::func_ddl_create(ddl_block* tmp_Label, QString comm_time, QString due_time)
{
    tmp_Label->setStyleSheet("QLabel{border:2px solid rgb(0, 255, 255);}");
    m_block.push_back(tmp_Label);
    tmp_Label->rank = m_block.size() - 1;
     //输入并在ddl块上显示当前时间
    QInputDialog type_in_name(tmp_Label);
    QString tmp_name = type_in_name.getText(tmp_Label, "name", "please type in ddl name", QLineEdit::Normal);
    tmp_Label->SetName(tmp_name);
    tmp_Label->setText(tmp_name);
    tmp_Label->setFont(QFont("Hack", 16));
    tmp_Label->setAlignment(Qt::AlignCenter);
//    //初始化ddl
    // tmp_Label->setDDL("UNKNOWN", comm_time,
    //                            due_time, "PLAIN", "NULL",
    //                            0, 0.0, "PREV","NEXT");

    // 将当前ddl模块的show_tasks信号与其slot_tasks槽连接
    connect(tmp_Label, SIGNAL(show_tasks()), tmp_Label, SLOT(slot_tasks()));
    connect(tmp_Label, SIGNAL(getInt(int)), this, SLOT(slot_delete(int)));
    connect(tmp_Label, SIGNAL(getInt_succ(int)), this, SLOT(slot_succ(int)));
    connect(tmp_Label, SIGNAL(getInt_prev(int)), this, SLOT(slot_prev(int)));
    tmp_Label->show();
    // 下面开始设置ddl_block出现的菜单
    tmp_Label->setContextMenuPolicy(Qt::CustomContextMenu);
    //给信号设置相应的槽函数
    connect(tmp_Label, &QLabel::customContextMenuRequested, [=](const QPoint &pos)
    {
        tmp_Label->m_pMenu->exec(QCursor::pos());
    });
}

// 槽：新建ddl模块
void MainWindow::create_ddl(){
     //新建DDL指针并设置参数，维护DDL序列,并且初始化ddl
    ddl_block *tmp_Label = new ddl_block(this->m_scrollWidget);
    //测试版
   QString comm_time = "2022-04-02 18:00:00";
   QString due_time = "2022-04-04 00:00:00";
   // QInputDialog type_in_commence(tmp_Label);
    // QString comm_time = type_in_commence.getText(tmp_Label, "comm_time",
    //                     "please type in commence time\n format:yyyy-MM-dd hh:mm:ss", QLineEdit::Normal);
    // QInputDialog type_in_due(tmp_Label);
    // QString due_time = type_in_due.getText(tmp_Label, "due_time",
    //                      "please type in due time\n format:yyyy-MM-dd hh:mm:ss", QLineEdit::Normal);
    QDateTime begin_time = QDateTime::fromString(comm_time, "yyyy-MM-dd hh:mm:ss");
    QDateTime end_time = QDateTime::fromString(due_time, "yyyy-MM-dd hh:mm:ss");
    QDateTime curr_time = QDateTime::currentDateTime();
    int width = 0;
    int height = 0;
    func_ddl_create(tmp_Label, comm_time, due_time);
    tmp_Label->line_rank = DDL_lines_number;
    this->number_each_line[tmp_Label->line_rank]++;
    DDL_lines_number++;
    tmp_Label->setGeometry(200 + tmp_Label->line_rank * 200, 1080 - 200 * curr_time.secsTo(end_time) / 1440 / 60, 200, begin_time.secsTo(end_time) * 200 / 1440 / 60);
}

//按秩删除ddl并维护ddl序列
void MainWindow::slot_delete(int rank){

    // 判断early exit条件
    if(m_block.size() == 0)return;
    this->m_block[rank]->hide();//clear out the GUI of that ddl
    // 给前驱后继擦屁股
    if(m_block[rank]->GetPrev() != "PREV")
    {
        m_block[rank]->SetPrev("PREV");
    }
    if(m_block[rank]->GetNext() != "NEXT")
    {
        m_block[rank]->SetNext("NEXT");
    }
    //this->m_block[rank]->Button_delete->hide();

    int deleted_linerank = this->m_block[rank]->line_rank;//记录被灭绝的line's rank
    //调整位置
    m_block.erase(m_block.begin() + rank);
    for(int i = 0; i < m_block.size(); i++)
    {
        m_block[i]->rank = i;
    }
    this->number_each_line[deleted_linerank]--;

    //如果这个ddl删除导致了少了一条line
    if(this->number_each_line[deleted_linerank] == 0)
    {
        for(int i = deleted_linerank; i < DDL_lines_number - 1; i++)//重新计数
            number_each_line[i] = 0;
        for(int i = 0; i < m_block.size(); i++)//对于每一个ddl
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
        DDL_lines_number--;
    }
    for(int i = 0; i < m_block.size(); i++)
    {
        this->m_block[i]->setGeometry(200 + this->m_block[i]->line_rank * 200,
                                         this->m_block[i]->y(), 200, this->m_block[i]->height());
    }
}

// 为当前DDl设置后继
void MainWindow::slot_succ(int rank){
    ddl_block *tmp_Label = new ddl_block(this->m_scrollWidget);
    QString comm_time = "2022-04-05 18:00:00";
   QString due_time = "2022-04-07 00:00:00";
   // QInputDialog type_in_commence(tmp_Label);
    // QString comm_time = type_in_commence.getText(tmp_Label, "comm_time",
    //                     "please type in commence time\n format:yyyy-MM-dd hh:mm:ss", QLineEdit::Normal);
    // QInputDialog type_in_due(tmp_Label);
    // QString due_time = type_in_due.getText(tmp_Label, "due_time",
    //                      "please type in due time\n format:yyyy-MM-dd hh:mm:ss", QLineEdit::Normal);
    QDateTime begin_time = QDateTime::fromString(comm_time, "yyyy-MM-dd hh:mm:ss");
    QDateTime end_time = QDateTime::fromString(due_time, "yyyy-MM-dd hh:mm:ss");
    QDateTime curr_time = QDateTime::currentDateTime();
    func_ddl_create(tmp_Label, comm_time, due_time);
    tmp_Label->line_rank = m_block[rank]->line_rank;//后继应该与被后继的在同一line中
    this->number_each_line[tmp_Label->line_rank]++;
    tmp_Label->setGeometry(m_block[rank]->x(), 1080 - 200 * curr_time.secsTo(end_time) / 1440 / 60, 200, begin_time.secsTo(end_time) * 200 / 1440 / 60);
    m_block[rank]->SetNext(QString::number(m_block.size() - 1, 10));//原来的ddl的后继的序号是新的ddl的序号
    tmp_Label->SetPrev(QString::number(rank, 10));//新的ddl的前驱的序号是原来的ddl
}

void MainWindow::slot_prev(int rank)
{
    ddl_block *tmp_Label = new ddl_block(this->m_scrollWidget);
    QString comm_time = "2022-03-29 18:00:00";
   QString due_time = "2022-03-30 00:00:00";
   // QInputDialog type_in_commence(tmp_Label);
    // QString comm_time = type_in_commence.getText(tmp_Label, "comm_time",
    //                     "please type in commence time\n format:yyyy-MM-dd hh:mm:ss", QLineEdit::Normal);
    // QInputDialog type_in_due(tmp_Label);
    // QString due_time = type_in_due.getText(tmp_Label, "due_time",
    //                      "please type in due time\n format:yyyy-MM-dd hh:mm:ss", QLineEdit::Normal);
    QDateTime begin_time = QDateTime::fromString(comm_time, "yyyy-MM-dd hh:mm:ss");
    QDateTime end_time = QDateTime::fromString(due_time, "yyyy-MM-dd hh:mm:ss");
    QDateTime curr_time = QDateTime::currentDateTime();
    func_ddl_create(tmp_Label, comm_time, due_time);
    tmp_Label->line_rank = m_block[rank]->line_rank;//后继应该与被后继的在同一line中
    this->number_each_line[tmp_Label->line_rank]++;
    tmp_Label->setGeometry(m_block[rank]->x(), 1080 - 200 * curr_time.secsTo(end_time) / 1440 / 60, 200, begin_time.secsTo(end_time) * 200 / 1440 / 60);
    m_block[rank]->SetPrev(QString::number(m_block.size() - 1, 10));//原来的ddl的后继的序号是新的ddl的序号
    tmp_Label->SetNext(QString::number(rank, 10));//新的ddl的前驱的序号是原来的ddl
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

