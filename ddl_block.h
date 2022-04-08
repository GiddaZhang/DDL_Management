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
#include <QMenu>
#include <QInputDialog>
#include <stdlib.h>
#include <time.h>
#include "ddl.h"
#include "listwidget.h"
#include "button_delete.h"
#include "button_new.h"
#include "button_next.h"
#include "workingfilelistitem.h"
#include "button_prev.h"
#include "notelistitem.h"


class ddl_block : public QLabel, public DDL
{
    Q_OBJECT
public:
    explicit ddl_block(QWidget *parent = nullptr);
    explicit ddl_block(DDL&, QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *ev) override;
    int rank;               //记录其在数组中的序号
    int line_rank;          //记录其line编号
    int m_x;
    int m_y;
    int m_length;
    int m_width;            //用于存储每个ddl的位置及大小参数
    void setParameters(int x, int y, int Length, int width);
    virtual ~ddl_block();
    button_delete *Button_delete;
    button_next *Button_next;
    button_prev *Button_prev;

    QMenu* m_pMenu;
    QAction* m_act[5];              // 菜单中的选项：工作，删除，留言，添加后继

    QWidget* m_FileWidget;          // 工作文件窗口
    QListWidget* m_ListWidget;      // 工作文件列表
    QAction* m_act_workingFile[3];
    QMenu* m_pMenu_workingFile;
    QWidget* m_NoteWidget;          // 留言窗口
    QListWidget* m_NoteListWidget;  // 留言列表

    void SetWorkingFileSpace();     // 设置工作文件界面
    void ShowWorkingFileSpace();    // 显示工作文件界面
    void SetNoteSpace();            // 设置留言界面
    void ShowNoteSpace();           // 显示留言界面

    void SetColor();                // 设置颜色

protected:
    void dragEnterEvent(QDragEnterEvent *e);    // 拖动文件用到的信号1
    void dropEvent(QDropEvent *e);              // 拖动文件用到的信号2(这两个信号都需要把文件拖动到ddl_block区域)

private:

signals:
    void deleteddl();
    void show_tasks();
    void getInt(int);
    void getInt_succ(int);
    void getInt_prev(int);
public slots:
    void slot_delete();
    void slot_tasks();
    void OnClickedPopMenu();                 // 点击菜单选项的槽函数
    void OnWorkingFileMenu(const QPoint &pos);
    void slot_openFile(QListWidgetItem *item);   // 点击工作文件路径的槽函数
    void slot_deleteFile();
    void slot_openFileMenu();
    void slot_saveFile();
    void slot_openAll();                     // 点击一键打开文件的槽函数
    void slot_saveAll();                     // 点击一键存储文件的槽函数
    void slot_addNote();
    void slot_voidToint(int rank);
    void emit_interchange();
    void emit_interchange_succ();
    void emit_interchange_prev();
};

#endif // DDL_BLOCK_H
