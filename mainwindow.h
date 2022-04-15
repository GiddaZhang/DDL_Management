#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QScrollArea>
#include <QScrollBar>
#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <memory>
#include <QCloseEvent>
#include <vector>
#include <QFile>
#include <QMouseEvent>
#include "ddl.h"
#include "axispainter.h"
#include "ddl_block.h"
#include "button.h"
#include "button_new.h"
#include "button_next.h"
#include "button_prev.h"
#include "button_new_next.h"
#include "button_next_succ.h"
#include "primarymenu.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    button_new *m_button;
    vector<ddl_block*> m_block;
    void func_ddl_create(ddl_block* tmp_Label,
                         QDateTime com,
                         QDateTime due,
                         QString name = "Default");
    void keyPressEvent(QKeyEvent *event) override;

    // 以下为界面初始化
    void windowInit();       // 初始化窗口名称、大小
    void menuInit();         // 初始化菜单栏和菜单项
    void scrollBarInit();    // 初始化滚动条
    void scrollAreaInit();   // 初始化滚动区域
    void fileInit();         // 读取存档初始化
    void QssInit();          // 初始化Qss样式表

    // 下面带auto的函数和孙哥的差不多，不过是根据从文档里读到的数据新建，不是由用户决定，
    // 在孙哥的基础上去掉和用户交互的界面。
    int create_ddl_auto(DDL&);             // 创建之后返回rank
    void succ_ddl_auto(int, DDL&);         // 第一个参数是rank
    void prev_ddl_auto(int, DDL&);         // 第一个参数是rank
    bool isDDLexsited(QString);

    void closeEvent(QCloseEvent*);         // 关闭程序时触发的函数

    QDateTimeEdit *dateEdit;
    QDateTimeEdit *dateEdit_end;
    QDateTime dateTime;
    QDateTime dateTime_end;
    QLabel *bar_begin;
    QDateTime begin_time;
    QDateTime end_time;

    void showDDLBlock(ddl_block*, QDateTime, QDateTime);\

public slots:
    void create_ddl();
    void slot_delete(int rank);
    void slot_succ(int rank);
    void slot_prev(int rank);
    void onDateTimeChanged(QDateTime dateTime);
    void onDateTimeChanged_end(QDateTime dateTime);
    void type_in_end_time();
    void type_in_end_time_succ(int rank);
    void ddl_set_OK();
    void ddl_set_OK_succ(int rank);

protected:
    //void paintEvent(QPaintEvent *event);

private slots:

private:
    Ui::MainWindow *ui;
    QScrollArea* m_scrollArea;      // 滚动区域
    QWidget* m_scrollWidget;        // 滚动窗口（窗口大于区域）
    QMenuBar* m_menuBar;            // 最顶上的菜单栏
    QMenu* m_Menu;                  // 菜单项
    QAction* m_createDDLAction;     // 菜单动作：新建DDL
    // int DDL_number;                 // 记录当前有效的DDL的个数
    int DDL_lines_number;           //一组前驱后继算一条线
    int number_each_line[30] = {0};
};

#endif // MAINWINDOW_H
