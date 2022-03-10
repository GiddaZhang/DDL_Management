#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "axispainter.h"
#include "ddl_block.h"
#include "button.h"
#include "button_new.h"
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
    ddl_block *m_block[30];
    //QString *name_block[30];
    bool isOccupied[30] = {0};//记录对应位置是否有ddl，有就是true，没有就是false
    void keyPressEvent(QKeyEvent *event) override;

public slots:
    void create_ddl();
    void slot_delete(int rank);
    void slot_succ(int rank);
    void showContextMenu(const QPoint &pos); //右键显示一级菜单的槽函数
    void OnClickedPopMenu();//点击菜单选项的槽函数

protected:
    //void paintEvent(QPaintEvent *event);

private:
    Ui::MainWindow *ui;
    int DDL_number;         // 记录当前有效的ddl的个数
    int DDL_lines_number;//一组前驱后继算一条线
    int number_each_line[30] = {0};
};

#endif // MAINWINDOW_H
