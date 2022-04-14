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
    this->DDL_lines_number = 0;

    windowInit();       // 初始化窗口
    menuInit();         // 初始化菜单栏和菜单项
    scrollBarInit();    // 初始化滚动条
    scrollAreaInit();   // 初始化滚动区域
    QssInit();          // 初始化样式表
    fileInit();         // 初始化静态存档(ddl::all_ddl)与动态存档(mainwindow::m_block)
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

// 初始化窗口
void MainWindow::windowInit(){
    // 设置并显示主窗口UI
    ui->setupUi(this);
    this->setGeometry(20, 20, 1920, 1080);
    this->setWindowTitle("DDL_MANAGEMENT");
}

// 初始化菜单栏和菜单项
void MainWindow::menuInit(){
    // 初始化菜单栏和菜单项
    this->m_menuBar = this->menuBar();
    m_menuBar->setMinimumHeight(30);
    this->m_Menu = this->m_menuBar->addMenu("开始");
    this->m_createDDLAction = new QAction("新建DDL", this);
    this->m_Menu->addAction(m_createDDLAction);        // 将菜单项添加到子菜单
    // 添加菜单响应函数
    connect(m_createDDLAction, &QAction::triggered, this, &MainWindow::create_ddl);
    //修改属性，设置为自定义菜单模式
    setContextMenuPolicy(Qt::CustomContextMenu);
}

// 初始化滚动条
void MainWindow::scrollBarInit(){
    // 初始化滚动条
    this->m_scrollArea = new QScrollArea(this);
    m_scrollArea->setBackgroundRole(QPalette::Light);                   // 背景色
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);    // 只让竖直滚动
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setGeometry(0, 30, 1920, 1080);
}

// 初始化滚动区域
void MainWindow::scrollAreaInit(){
    // 初始化滚动区域
    this->m_scrollWidget = new QWidget(this);
    this->m_scrollWidget->setGeometry(0, 0, 1920, 3000);
    m_scrollArea->setWidget(this->m_scrollWidget);
}

// 初始化样式表
void MainWindow::QssInit(){
    QFile* qssFile = new QFile(":/myQss.qss");
    // 只读方式打开该文件
    qssFile->open(QFile::ReadOnly);
    if(qssFile->isOpen()) {
        qDebug() << 1;
    }
    // 读取文件全部内容
    QString styleSheet = QString(qssFile->readAll());
    // 为QApplication设置样式表
    qApp->setStyleSheet(styleSheet);
    qssFile->close();
    delete qssFile;
}

// 读存档函数：初始化静态存档(ddl::all_ddl)并按照静态存档初始化动态存档(mainwindow::m_block)
void MainWindow::fileInit(){
    DDL::LoadFromFile();// 先将文档信息存储在DDL的静态变量中
    vector<shared_ptr<DDL>> prevDDL = DDL::GetAllDDLPtr();  // 把存档中的DDL存在prevDDL里
    // 遍历已存储的DDL
    int curRank;
    ddl_block* curDDL;
    for(auto it = prevDDL.begin(); it != prevDDL.end(); it++) {
        if(!isDDLexsited((*it)->GetName())) {
            curRank = this->create_ddl_auto((**it)); //本步骤用当前ddl更新了m_block，并返回新建block的秩

            // 下面创建前驱后继
            curDDL = m_block[curRank]; // 首先选定当前ddlblock
            if((*it)->GetPrev() != "PREV") {// 如果有前驱
                curDDL->SetPrev((*it)->GetPrev());// 那么当前ddlblock根据ddl情况设定前驱
                for(auto it = prevDDL.begin(); it != prevDDL.end(); it++) {
                    // 同时在ddl列中搜索该前驱
                    if((*it)->GetName() == curDDL->GetPrev()) {
                        this->prev_ddl_auto(curRank, (**it)); // 直接在m_block中新建prev
                        // 注意这一举动会超前主循环中prev的block应该被建立的步骤，但通过名字判断可以避免重复
                    }
                }
            }
            // 创建后继的过程完全一致
            if((*it)->GetNext() != "NEXT") {
                curDDL->SetNext((*it)->GetNext());
                for(auto it = prevDDL.begin(); it != prevDDL.end(); it++) {
                    if((*it)->GetName() == curDDL->GetNext()) {
                        this->succ_ddl_auto(curRank, (**it));
                    }
                }
            }
        }
        else {
            continue;
        }
    }
}

// 用户和读存档create操作的公用底层，会更新m_block
void MainWindow::func_ddl_create(ddl_block* tmp_Label, QDateTime com,
                                 QDateTime due, QString name)
{
    if(name == "Default") {
        this->bar_begin->hide();
        this->dateEdit_end->hide();
    }
    // tmp_Label是一个ddl_block指针，在上层步骤中只是被创建但未被完善。本步完善后存入m_block
    tmp_Label->rank = m_block.size(); // 为block设定秩
    QString tmp_name;
    // 判断caller是用户还是读存档函数。当name是"Default"，表明用户在调用
    if(name == "Default") {
        //输入并在ddl块上显示当前时间
        QInputDialog type_in_name(tmp_Label);
        tmp_name = type_in_name.getText(tmp_Label, "name", "please type in ddl name", QLineEdit::Normal);

        // 用caller（create_ddl)和前文已经预处理好的三个参数更新block
        tmp_Label->SetName(tmp_name);
        tmp_Label->SetCommence(com.toString("yyyy-MM-dd hh:mm:ss"));
        tmp_Label->SetDue(due.toString("yyyy-MM-dd hh:mm:ss"));

        // 动态地更新静态变量，向其中加入新建立的ddl
        DDL* tmp_ddl = new DDL(tmp_name, com.toString("yyyy-MM-dd hh:mm:ss"),
                               due.toString("yyyy-MM-dd hh:mm:ss"));
    }
    else {// 若读存档在调用
        // 由于读存档时用ddl初始化label，已经将时间信息读入，所以只需要更新名称
        tmp_name = name;
        tmp_Label->SetName(tmp_name, true);
        // 同时也不需要再更新静态
    }
    // 为block设置显示格式
    tmp_Label->setText(tmp_name);
    tmp_Label->setAlignment(Qt::AlignCenter);

    m_block.push_back(tmp_Label);

    // 将当前ddl模块的信号与槽连接
    connect(tmp_Label, SIGNAL(show_tasks()), tmp_Label, SLOT(slot_tasks()));
    connect(tmp_Label, SIGNAL(getInt(int)), this, SLOT(slot_delete(int)));
    connect(tmp_Label, SIGNAL(getInt_succ(int)), this, SLOT(slot_succ(int)));
    connect(tmp_Label, SIGNAL(getInt_prev(int)), this, SLOT(slot_prev(int)));
    tmp_Label->show();
    // 下面开始设置ddl_block出现的菜单
    tmp_Label->setContextMenuPolicy(Qt::CustomContextMenu);
    //给信号设置相应的槽函数
    connect(tmp_Label, &QLabel::customContextMenuRequested, [=](const QPoint &pos){
        tmp_Label->m_pMenu->exec(QCursor::pos());
    });
}

// 用户调用的新建block接口
void MainWindow::create_ddl(){
    //新建block
    ddl_block *tmp_Label = new ddl_block(this->m_scrollWidget);

//    测试版
//    QString comm_time = "2022-04-04 18:00:00";
//    QString due_time = "2022-04-06 00:00:00";

    // 提供窗口并接收用户对开始时间的输入，根据格式转换为字符串
    dateEdit = new QDateTimeEdit(QDate::currentDate(), this);
    dateEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    dateEdit->setCalendarPopup(true);  // 日历弹出
    dateEdit->show();
    begin_time = dateEdit->dateTime();
    dateEdit->setGeometry(500, 500, 400, 200);

    bar_begin = new QLabel(this);
    bar_begin->setGeometry(500, 300, 400, 200);
    bar_begin->setText("请输入ddl的开始时间");
    bar_begin->show();

    button_new *button_begin = new button_new(this);
    button_begin->setGeometry(500, 700, 400, 200);
    button_begin->show();
    button_begin->setText("确认");

    connect(dateEdit, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(onDateTimeChanged(QDateTime)));
    connect(button_begin, SIGNAL(newddl()), this, SLOT(type_in_end_time()));


    // QInputDialog type_in_commence(tmp_Label);
    // QString comm_time = type_in_commence.getText(tmp_Label, "comm_time",
    //                      "please type in commence time\n format:yyyy-MM-dd hh:mm:ss", QLineEdit::Normal);
    // QDateTime begin_time = QDateTime::fromString(comm_time, "yyyy-MM-dd hh:mm:ss");

    // // 提供窗口并接收用户对结束时间的字符串输入，根据格式转换为日期对象
    // QInputDialog type_in_due(tmp_Label);
    // QString due_time = type_in_due.getText(tmp_Label, "due_time",
    //                       "please type in due time\n format:yyyy-MM-dd hh:mm:ss", QLineEdit::Normal);
    // QDateTime end_time = QDateTime::fromString(due_time, "yyyy-MM-dd hh:mm:ss");

    // 将预处理好的ddl_block和两个日期对象传给block构建函数的公用底层func_ddl_create
    // func_ddl_create(tmp_Label, begin_time, end_time);

    // // 处理前驱后继链条数量。当前链条退化为一个点
    // tmp_Label->line_rank = DDL_lines_number;
    // this->number_each_line[tmp_Label->line_rank]++;
    // DDL_lines_number++;

    // // 根据当前时间和结束时间为ddlblock在界面上确定位置
    // QDateTime curr_time = QDateTime::currentDateTime();
    // tmp_Label->setGeometry(200 + tmp_Label->line_rank * 200,
    //                        200 * curr_time.secsTo(begin_time) / 1440 / 60, 200,
    //                        begin_time.secsTo(end_time) * 200 / 1440 / 60);
    //qDebug() << tmp_Label->y();
}

// 读存档调用的新建block接口
int MainWindow::create_ddl_auto(DDL& ddl){
    //新建block
    ddl_block *tmp_Label = new ddl_block(ddl, this->m_scrollWidget);// 此处用ddl建立block，内部信息直接拷贝，后文省略很多操作

    // 将预处理好的ddl_block传给block构建函数的公用底层func_ddl_create
    func_ddl_create(tmp_Label, QDateTime::currentDateTime(),
                    QDateTime::currentDateTime(), ddl.GetName());

    // 处理前驱后继链条数量。当前链条退化为一个点
    tmp_Label->line_rank = DDL_lines_number;
    this->number_each_line[tmp_Label->line_rank]++;
    DDL_lines_number++;

    // 根据当前时间和结束时间为ddlblock在界面上确定位置
    this->showDDLBlock(tmp_Label, ddl.GetComm(), ddl.GetDue());
    return tmp_Label->rank;// 当前block的秩在func中修改，现在返回给读存档函数
}

// 用户调用的删除block接口
void MainWindow::slot_delete(int rank){
    if(m_block.size() == 0){// 判断early exit条件：m_block中无对象
        return;
    }
    this->m_block[rank]->hide();// 关闭当前block的界面

    // 处理被删除block的前驱后继
    if(m_block[rank]->GetPrev() != "PREV"){
        m_block[rank]->SetPrev("PREV");
    }
    if(m_block[rank]->GetNext() != "NEXT"){
        m_block[rank]->SetNext("NEXT");
    }

    int deleted_linerank = this->m_block[rank]->line_rank;//记录被删除的block的链路
    DDL::RemoveDDL(m_block[rank]->GetName());// 更新静态变量：从中删除同名对象
    m_block.erase(m_block.begin() + rank);// 更新动态变量：从中删除

    for(unsigned int i = 0; i < m_block.size(); i++){
        m_block[i]->rank = i;// 重新设定block的秩
    }

    this->number_each_line[deleted_linerank]--;
    if(this->number_each_line[deleted_linerank] == 0){//如果这个ddl删除导致了少了一条链路
        for(int i = deleted_linerank; i < DDL_lines_number - 1; i++)//重新计数
            number_each_line[i] = 0;
        for(unsigned int i = 0; i < m_block.size(); i++){
            if(this->m_block[i]->line_rank > deleted_linerank){//如果他的linerank大于被删掉的
                this->m_block[i]->line_rank--;
                number_each_line[this->m_block[i]->line_rank]++;
            }
            this->m_block[i]->setGeometry(200 + this->m_block[i]->line_rank * 200,
                                          this->m_block[i]->y(), 200, this->m_block[i]->height());
        }
        this->number_each_line[DDL_lines_number] = 0;
        DDL_lines_number--;
    }
    for(unsigned int i = 0; i < m_block.size(); i++){
        this->m_block[i]->setGeometry(200 + this->m_block[i]->line_rank * 200,
                                         this->m_block[i]->y(), 200, this->m_block[i]->height());
    }
}

// 用户调用的设置后继接口
void MainWindow::slot_succ(int rank){
    // 新建block
    ddl_block *tmp_Label = new ddl_block(this->m_scrollWidget);
//    QString comm_time = "2022-04-06 18:00:00";
//    QString due_time = "2022-04-07 00:00:00";

    // 提供窗口并接收用户对开始时间的输入，根据格式转换为字符串
    QInputDialog type_in_commence(tmp_Label);
    QString comm_time = type_in_commence.getText(tmp_Label, "comm_time",
                         "please type in commence time\n format:yyyy-MM-dd hh:mm:ss", QLineEdit::Normal);
    QDateTime begin_time = QDateTime::fromString(comm_time, "yyyy-MM-dd hh:mm:ss");

    // 提供窗口并接收用户对结束时间的输入，根据格式转换为字符串
    QInputDialog type_in_due(tmp_Label);
    QString due_time = type_in_due.getText(tmp_Label, "due_time",
                          "please type in due time\n format:yyyy-MM-dd hh:mm:ss", QLineEdit::Normal);
    QDateTime end_time = QDateTime::fromString(due_time, "yyyy-MM-dd hh:mm:ss");

    // 将预处理好的ddl_block和两个日期对象传给block构建函数的公用底层func_ddl_create
//    func_ddl_create(tmp_Label, begin_time, end_time);

    // 将后继与原block设置在同一链路中
    tmp_Label->line_rank = m_block[rank]->line_rank;
    this->number_each_line[tmp_Label->line_rank]++;

    // 根据当前时间和结束时间为ddlblock在界面上确定位置
    QDateTime curr_time = QDateTime::currentDateTime();
    tmp_Label->setGeometry(m_block[rank]->x(),
                           200 * curr_time.secsTo(begin_time) / 1440 / 60,
                           200, begin_time.secsTo(end_time) * 200 / 1440 / 60);
    qDebug() << tmp_Label->y();
    // 在动态变量中为原block和新block互设前驱后继
    m_block[rank]->SetNext(tmp_Label->GetName());// 将原block后继设置为新block的名称
    tmp_Label->SetPrev(m_block[rank]->GetName());// 将新block前驱设置为原block的名称

    // 在静态变量中为原block和新block互设前驱后继
    shared_ptr<DDL> thisBlock = DDL::GetDDLPtr(tmp_Label->GetName());
    thisBlock->SetPrev(m_block[rank]->GetName());// 将新block前驱设置为原block的名称
    thisBlock = DDL::GetDDLPtr(m_block[rank]->GetName());
    thisBlock->SetNext(tmp_Label->GetName());// 将原block后继设置为新block的名称
}

// 读存档调用的设置后继接口
void MainWindow::succ_ddl_auto(int rank, DDL& ddl){
    //新建block
    ddl_block *tmp_Label = new ddl_block(ddl, this->m_scrollWidget);// 此处用ddl建立block，内部信息直接拷贝，后文省略很多操作

    // 将预处理好的ddl_block传给block构建函数的公用底层func_ddl_create
    func_ddl_create(tmp_Label, QDateTime::currentDateTime(),
                    QDateTime::currentDateTime(), ddl.GetName());

    // 将后继与原block设置在同一链路中
    tmp_Label->line_rank = m_block[rank]->line_rank;
    this->number_each_line[tmp_Label->line_rank]++;

    // 根据当前时间和结束时间为ddlblock在界面上确定位置
    QDateTime curr_time = QDateTime::currentDateTime();
    tmp_Label->setGeometry(m_block[rank]->x(),
                           200 * curr_time.secsTo(ddl.GetComm()) / 1440 / 60,
                           200, ddl.GetComm().secsTo(ddl.GetDue()) * 200 / 1440 / 60);
}

// 用户调用的设置前驱接口
void MainWindow::slot_prev(int rank){
    // 新建block
    ddl_block *tmp_Label = new ddl_block(this->m_scrollWidget);
//    QString comm_time = "2022-04-03 12:00:00";
//    QString due_time = "2022-04-04 00:00:00";

    // 提供窗口并接收用户对开始时间的输入，根据格式转换为字符串
    QInputDialog type_in_commence(tmp_Label);
    QString comm_time = type_in_commence.getText(tmp_Label, "comm_time",
                         "please type in commence time\n format:yyyy-MM-dd hh:mm:ss", QLineEdit::Normal);
    QDateTime begin_time = QDateTime::fromString(comm_time, "yyyy-MM-dd hh:mm:ss");

    // 提供窗口并接收用户对结束时间的输入，根据格式转换为字符串
    QInputDialog type_in_due(tmp_Label);
    QString due_time = type_in_due.getText(tmp_Label, "due_time",
                          "please type in due time\n format:yyyy-MM-dd hh:mm:ss", QLineEdit::Normal);
    QDateTime end_time = QDateTime::fromString(due_time, "yyyy-MM-dd hh:mm:ss");

    // 将预处理好的ddl_block和两个日期对象传给block构建函数的公用底层func_ddl_create
    func_ddl_create(tmp_Label, begin_time, end_time);

    // 将前驱与原block设置在同一链路中
    tmp_Label->line_rank = m_block[rank]->line_rank;
    this->number_each_line[tmp_Label->line_rank]++;

    // 根据当前时间和结束时间为ddlblock在界面上确定位置
    QDateTime curr_time = QDateTime::currentDateTime();
    tmp_Label->setGeometry(m_block[rank]->x(),
                           200 * curr_time.secsTo(begin_time) / 1440 / 60, 200,
                           begin_time.secsTo(end_time) * 200 / 1440 / 60);
    qDebug() << tmp_Label->y();

    // 在动态变量中为原block和新block互设前驱后继
    m_block[rank]->SetPrev(tmp_Label->GetName());// 将原block前驱设置为新block的名称
    tmp_Label->SetNext(m_block[rank]->GetName());// 将新block后继设置为原block的名称

    // 在静态变量中为原block和新block互设前驱后继
    shared_ptr<DDL> thisBlock = DDL::GetDDLPtr(tmp_Label->GetName());
    thisBlock->SetNext(m_block[rank]->GetName());// 将新block后继设置为原block的名称
    thisBlock = DDL::GetDDLPtr(m_block[rank]->GetName());
    thisBlock->SetPrev(tmp_Label->GetName());// 将原block前驱设置为新block的名称
}

// 读存档调用的设置前驱接口
void MainWindow::prev_ddl_auto(int rank, DDL& ddl){
    // 新建block
    ddl_block *tmp_Label = new ddl_block(ddl, this->m_scrollWidget);// 此处用ddl建立block，内部信息直接拷贝，后文省略很多操作

    // 将预处理好的ddl_block传给block构建函数的公用底层func_ddl_create
    func_ddl_create(tmp_Label, QDateTime::currentDateTime(),
                    QDateTime::currentDateTime(), ddl.GetName());

    // 将前驱与原block设置在同一链路中
    tmp_Label->line_rank = m_block[rank]->line_rank;
    this->number_each_line[tmp_Label->line_rank]++;

    // 根据当前时间和结束时间为ddlblock在界面上确定位置
    this->showDDLBlock(tmp_Label, begin_time, end_time);
//    QDateTime curr_time = QDateTime::currentDateTime();
//    tmp_Label->setGeometry(m_block[rank]->x(),
//                           200 * curr_time.secsTo(ddl.GetDue()) / 1440 / 60, 200,
//                           ddl.GetComm().secsTo(ddl.GetDue()) * 200 / 1440 / 60);
}

// 在m_block中判断ddl存在与否
bool MainWindow::isDDLexsited(QString name){
    for(auto it = m_block.begin();it != m_block.end();it++) {
        if((*it)->GetName() == name) {
            return true;
        }
    }
    return false;
}

// 关闭程序并将静态变量存入文档
void MainWindow::closeEvent(QCloseEvent *event){
    switch(QMessageBox::information(this, tr("Exit DDL_MANAGEMENT"),
           tr("是否退出？系统会自动保存数据。"), tr("是"), tr("否"), 0, 1)){
    case 0:{
        DDL::SaveToFile(); // 将静态变量中数据存入文档
        event->accept();
        break;
    }
    case 1:
    default:
        event->ignore();
        break;
    }
}

void MainWindow::onDateTimeChanged(QDateTime dateTime)
{
    this->dateTime = dateTime;
    qDebug() << this->dateTime;
}
void MainWindow::onDateTimeChanged_end(QDateTime dateTime)
{
    this->dateTime_end = dateTime;
    qDebug() << this->dateTime;
}
void MainWindow::type_in_end_time()
{
    this->begin_time = dateTime;
    this->bar_begin->setText("请输入ddl的截止时间");
    this->dateEdit->hide();

    dateEdit_end = new QDateTimeEdit(QDate::currentDate(), this);
    dateEdit_end->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    dateEdit_end->setCalendarPopup(true);  // 日历弹出
    dateEdit_end->setGeometry(500, 500, 400, 200);
    dateEdit_end->show();
    end_time = dateEdit->dateTime();
    connect(dateEdit_end, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(onDateTimeChanged_end(QDateTime)));

    button_next *button_end = new button_next(this);
    button_end->setGeometry(500, 700, 400, 200);
    button_end->show();
    button_end->setText("确认");
    connect(button_end, SIGNAL(ddl_end()), this, SLOT(ddl_set_OK()));
    //connect(dateEdit, SIGNAL(dateTimeChanged(QDateTime)), this, SLOT(onDateTimeChanged(QDateTime)));

}
void MainWindow::ddl_set_OK()
{
    this->end_time = dateTime_end;
    qDebug() << "shit";
    qDebug() << "begin_time" << begin_time;
    qDebug() << "end_time" << end_time;
    ddl_block *tmp_Label = new ddl_block(this->m_scrollWidget);
    // 处理前驱后继链条数量。当前链条退化为一个点
    tmp_Label->line_rank = DDL_lines_number;
    this->number_each_line[tmp_Label->line_rank]++;
    DDL_lines_number++;
    bar_begin->hide();
    dateEdit->hide();
    func_ddl_create(tmp_Label, begin_time, end_time);

    // 根据当前时间和结束时间为ddlblock在界面上确定位置
    this->showDDLBlock(tmp_Label, begin_time, end_time);
}

void MainWindow::showDDLBlock(ddl_block* block,
                              QDateTime begin_time, QDateTime end_time)
{
    QDateTime curr_time = QDateTime::currentDateTime();
    block->setGeometry(200 + block->line_rank * 200,
                           200 * curr_time.secsTo(begin_time) / 1440 / 60, 200,
                           begin_time.secsTo(end_time) * 200 / 1440 / 60);
}
