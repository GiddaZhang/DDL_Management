#ifndef WORKINGFILELISTITEM_H
#define WORKINGFILELISTITEM_H

#include <QListWidgetItem>
#include <QAction>
#include <QMenu>
#include "workingfile.h"

class WorkingFileListItem : public QListWidgetItem
{
public:
    WorkingFileListItem(QString);
    QAction* m_act[3];         // 菜单中的选项：打开，备份，删除
    QMenu* m_menu;
private:
    WorkingFile* m_file;
};

#endif // WORKINGFILELISTITEM_H
