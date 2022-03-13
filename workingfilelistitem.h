#ifndef WORKINGFILELISTITEM_H
#define WORKINGFILELISTITEM_H

//#include <QListWidgetItem>
#include <QAction>
#include <QMenu>
//#include <QObject>
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
#include "workingfile.h"

class WorkingFileListItem : public QListWidgetItem
{
public:
    WorkingFileListItem(QString);
    QAction* m_act[3];                  // 菜单中的选项：打开，备份，删除
    QMenu* m_menu;
    void saveData();                    // 备份文件
private:
    WorkingFile* m_file;
//public slots:
//    void OnClickedWorkingFileMenu();         // 右键工作文件路径显示菜单的槽函数
};

#endif // WORKINGFILELISTITEM_H
