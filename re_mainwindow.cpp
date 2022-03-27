#include "mainwindow.h"
//#include "re_mainwindow.h"
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
    m_button->setFont(QFont("Hack", 10));
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

void MainWindow::create_ddl(){
     //新建DDL指针并设置参数，维护DDL序列,并且初始化ddl
    ddl_block *tmp_Label = new ddl_block(this);
    tmp_Label->setStyleSheet("QLabel{border:2px solid rgb(0, 255, 255);}");
    //this->m_block[this->DDL_number] = tmp_Label;
    m_block.push_back(tmp_Label);
    tmp_Label->rank = m_block.size() - 1;
    tmp_Label->line_rank = DDL_lines_number;
    this->number_each_line[tmp_Label->line_rank]++;
    //DDL_number++;
    DDL_lines_number++;
    //初始化ddl

    //测试版
//    QString comm_time = "2022-03-14 18:00:00";
//    QString due_time = "2022-03-15 00:00:00";

    //人性化的让用户选择日期时间
//    QDateTimeEdit *time_selector = new QDateTimeEdit(QDateTime::currentDateTime(), this);
//    time_selector->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
//    time_selector->setCalendarPopup(true);
//    time_selector->setGeometry(400, 400, 400, 400);
//    time_selector->show();

    //QMessageBox *hint_time = new QMessageBox(this);
    //输入当前时间，格式为yyyy-MM-dd hh:mm:ss，存储在QString变量里
    QInputDialog type_in_commence(tmp_Label);
    QString comm_time = type_in_commence.getText(tmp_Label, "comm_time",
                        "please type in commence time\n format:yyyy-MM-dd hh:mm:ss", QLineEdit::Normal);
    QInputDialog type_in_due(tmp_Label);
    QString due_time = type_in_due.getText(tmp_Label, "due_time",
                         "please type in due time\n format:yyyy-MM-dd hh:mm:ss", QLineEdit::Normal);

     //输入并在ddl块上显示当前时间
    QInputDialog type_in_name(tmp_Label);
    QString tmp_name = type_in_name.getText(tmp_Label, "name", "please type in ddl name", QLineEdit::Normal);
    tmp_Label->m_ddl->SetName(tmp_name);
    tmp_Label->setText(tmp_name);
    tmp_Label->setFont(QFont("Hack", 16));
    tmp_Label->setAlignment(Qt::AlignCenter);

    //tmp_Label->setText(QString::number(tmp_Label->rank, 10) + QString::number(tmp_Label->line_rank, 10));


//    //初始化ddl
    tmp_Label->m_ddl = new DDL("UNKNOWN", comm_time,
                               due_time, "PLAIN", "NULL",
                               0, 0.0, "PREV","NEXT");
    //获得ddl持续时间，决定其在界面上的长度和位置
    QDateTime begin_time = QDateTime::fromString(comm_time, "yyyy-MM-dd hh:mm:ss");
    QDateTime end_time = QDateTime::fromString(due_time, "yyyy-MM-dd hh:mm:ss");
    QDateTime curr_time = QDateTime::currentDateTime();

    //qDebug() << begin_time.secsTo(end_time) * 200 / 1440 / 60;//获取ddl的长度（日）
    tmp_Label->setGeometry(200 + tmp_Label->line_rank * 200, 1080 - 200 * curr_time.secsTo(end_time) / 1440 / 60, 200, begin_time.secsTo(end_time) * 200 / 1440 / 60);
    //tmp_Label->setGeometry(400, 400, 400, 400);

    // 将当前ddl模块的show_tasks信号与其slot_tasks槽连接
    connect(tmp_Label, SIGNAL(show_tasks()), tmp_Label, SLOT(slot_tasks()));

    // 为ddl模块提供“删除”按钮
    //tmp_Label->Button_delete = new button_delete(tmp_Label);
//    tmp_Label->Button_delete->setGeometry(0, 0, 150, 75);
//    tmp_Label->Button_delete->setParameters(0, 0, 150, 75);
    //tmp_Label->Button_delete->setText("delete");
    //tmp_Label->Button_delete->show();
    // 将“删除”按钮的delete_ddl信号与主窗口的slot_delete槽连接
    //connect(tmp_Label->Button_delete, SIGNAL(delete_ddl(int)), this, SLOT(slot_delete(int)));
    connect(tmp_Label, SIGNAL(getInt(int)), this, SLOT(slot_delete(int)));
    connect(tmp_Label, SIGNAL(getInt_succ(int)), this, SLOT(slot_succ(int)));
    connect(tmp_Label, SIGNAL(getInt_prev(int)), this, SLOT(slot_prev(int)));



    // 为ddl模块提供“后继”按钮，操作同上
    //tmp_Label->Button_next = new button_next(tmp_Label);
//    tmp_Label->Button_next->setGeometry(150, 0, 150, 75);
    //tmp_Label->Button_next->setText("succ");
    //tmp_Label->Button_next->show();

    // 为ddl模块提供“前驱”按钮，操作同上
    //tmp_Label->Button_prev = new button_prev(tmp_Label);
    //tmp_Label->Button_prev->setGeometry(0, 150, 150, 75);
    //tmp_Label->Button_prev->setText("prev");
    //tmp_Label->Button_prev->show();

    // 记录此ddl模块的秩，在删除时维护ddl序列
    // tmp_Label->Button_delete->rank = DDL_number - 1;
    // tmp_Label->Button_next->rank = DDL_number - 1;
    // tmp_Label->Button_prev->rank = DDL_number - 1;
    // tmp_Label->rank = DDL_number - 1;

    // connect(tmp_Label->Button_next, SIGNAL(next_ddl(int)), this, SLOT(slot_succ(int)));
    // connect(tmp_Label->Button_prev, SIGNAL(prev_ddl(int)), this, SLOT(slot_prev(int)));
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
    // tmp_Label->Button_prev->hide();
    // tmp_Label->Button_next->hide();
    // tmp_Label->Button_delete->hide();
}