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
#include <vector>
#include "axispainter.h"
#include "ddl_block.h"
#include "button.h"
#include "button_new.h"
#include "button_next.h"
#include "button_prev.h"
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
    //ddl_block *m_block[30];
    vector<ddl_block*> m_block;
    //QString *name_block[30];
    //bool isOccupied[30] = {0};//记录对应位置是否有ddl，有就是true，没有就是false
    void func_ddl_create(ddl_block* tmp_Label, QString comm_time, QString due_time);
    void keyPressEvent(QKeyEvent *event) override;
//    void func_ddl_create(ddl_block* tmp_Label, QString comm_time, QString due_time);

public slots:
    void create_ddl();
    void slot_delete(int rank);
    void slot_succ(int rank);
    void slot_prev(int rank);
//    void slot_openWorkingFile();

protected:
    //void paintEvent(QPaintEvent *event);

private slots:

private:
    Ui::MainWindow *ui;
    QScrollArea* m_scrollArea;      // 滚动区域
    QWidget* m_scrollWidget;        // 滚动窗口（窗口大于区域）
    QMenuBar* m_menuBar;            // 最顶上的菜单栏
    QMenu* m_Menu;                  // 菜单项
    QAction* m_createDDLAction;       // 菜单动作：新建DDL
    // int DDL_number;                 // 记录当前有效的DDL的个数
    int DDL_lines_number;           //一组前驱后继算一条线
    int number_each_line[30] = {0};
};

#endif // MAINWINDOW_H
