#ifndef DDL_BLOCK_H
#define DDL_BLOCK_H

#include <QObject>
#include <QWidget>
#include <QMainWindow>
#include <QGroupBox>
#include <QLabel>
#include <QKeyEvent>
#include <QDebug>
#include <QMouseEvent>
#include <QMessageBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QList>
#include <QDragEnterEvent>
#include <QFile>
#include <QMimeData>
#include "ddl.h"
#include "button_delete.h"
#include "button_new.h"
#include "button_next.h"
#include "primarymenu.h"
#include "workingfilelistitem.h"


class ddl_block : public QLabel
{
    Q_OBJECT
public:
    explicit ddl_block(QMainWindow *parent = nullptr);
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void setddl(int degree, bool state, bool turn,
                QString time, QString des,
                QString dur,
                DDL* prev, DDL* next);
    int rank;//记录其在数组中的序号
    int line_rank;//记录其的line编号
    int m_x;
    int m_y;
    int m_length;
    int m_width;//用于存储每个ddl的位置及大小参数
    void setParameters(int x, int y, int Length, int width);
    virtual ~ddl_block();
    button_delete *Button_delete;
    button_next *Button_next;

    DDL* m_ddl;                     // DDL成员
    PrimaryMenu* m_pMenu;
    QAction* m_act[5];              // 菜单中的选项：工作，删除，留言，添加后继
    QWidget* m_FileWidget;          // 工作文件窗口
    QListWidget* m_ListWidget;      // 工作文件列表

    void SetWorkingFileSpace();     // 设置工作文件界面
    void ShowWorkingFileSpace();    // 显示工作文件界面

protected:
    void dragEnterEvent(QDragEnterEvent *e);    // 拖动文件用到的信号1
    void dropEvent(QDropEvent *e);              // 拖动文件用到的信号2(这两个信号都需要把文件拖动到ddl_block区域)

private:

signals:
    void deleteddl();
    void show_tasks();
public slots:
    void slot_delete();
    void slot_tasks();
    void OnClickedPopMenu();                 // 点击菜单选项的槽函数
    void slot_open(QListWidgetItem *item);   // 点击工作文件路径的槽函数

};

#endif // DDL_BLOCK_H
