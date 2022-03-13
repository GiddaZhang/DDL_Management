#include "workingfilelistitem.h"

WorkingFileListItem::WorkingFileListItem(QString path) : QListWidgetItem(path)
{
    this->m_file = new WorkingFile(path);
    // 初始化菜单
    this->m_menu = new QMenu;

    this->m_act[0] = new QAction("打开");
    this->m_act[1] = new QAction("备份");
    this->m_act[2] = new QAction("删除");

    // 把QAction对象添加到菜单上
    for(int i = 0; i < 3; i++) {
        this->m_menu->addAction(this->m_act[i]);
    }

    // 设置点击后发送的数据
    for(int i = 0; i < 3; i++) {
        this->m_act[i]->setData(i + 1);
    }

    // 连接鼠标右键点击信号
//    for(int i = 0; i < 3; i++) {
//        QObject::connect(this->m_act[i], &QAction::triggered, this, &WorkingFileListItem::OnClickedWorkingFileMenu);
    //    }
}

void WorkingFileListItem::saveData()
{
    this->m_file->SaveToFolder(this->m_file->GetFilePath());
}


