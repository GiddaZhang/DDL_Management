#include "ddl_block.h"

ddl_block::ddl_block(QMainWindow *parent) : QLabel(parent)
{
    this->m_ddl = new DDL;      // 初始化DDL

    m_ddl->AddPath("D:/Desktop/2.txt");

    // 初始化菜单
    this->m_pMenu = new PrimaryMenu(parent);

    this->m_act[0] = new QAction("工作", this);
    this->m_act[1] = new QAction("删除", this);
    this->m_act[2] = new QAction("留言", this);
    this->m_act[3] = new QAction("添加后继", this);
    this->m_act[4] = new QAction("添加前驱", this);

    // 把QAction对象添加到菜单上
    for(int i = 0; i < 5; i++) {
        this->m_pMenu->addAction(this->m_act[i]);
    }

    //设置点击后发送的数据
    for(int i = 0; i < 5; i++) {
        this->m_act[i]->setData(i + 1);
    }

    // 连接鼠标右键点击信号
    for(int i = 0; i < 5; i++) {
        connect(this->m_act[i], SIGNAL(triggered()), this, SLOT(OnClickedPopMenu()));
    }

    this->setAcceptDrops(true);         // 启用放下操作
    this->SetWorkingFileSpace();        // 初始化工作文件窗口
}

void ddl_block::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W)
    {

    }
}

void ddl_block::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::RightButton)
    {
        emit show_tasks();
    }
}

void ddl_block::setddl(int degree, bool state, bool turn,
                       QString time, QString des,
                       QString dur,
                       DDL* prev, DDL* next)
{
    //this->m_ddl.SetDDL(degree, state, turn, time, des, dur, prev, next);
}

void ddl_block::setParameters(int x, int y, int Length, int width)
{
    this->m_x = x;
    this->m_y = y;
    this->m_length = Length;
    this->m_width = width;
}

ddl_block::~ddl_block()
{

}

void ddl_block::slot_delete()
{

}

void ddl_block::slot_tasks()
{
    //qDebug() << "Cyka";
}

void ddl_block::OnClickedPopMenu()
{
    QAction *pEven = qobject_cast<QAction *>(this->sender());

    //获取发送信息
    int iType = pEven->data().toInt();
    switch (iType)
    {
    case 1:
        this->ShowWorkingFileSpace();
        break;
    case 2:
        emit_interchange();
        break;
    case 3:
        QMessageBox::about(this, "留言功能开发中", pEven->text());
        break;
    case 4:
        emit_interchange_succ();
        break;
    case 5:
        emit_interchange_prev();
        break;
    default:
        break;
    }
}

void ddl_block::dragEnterEvent(QDragEnterEvent *e)
{
    //对拖放事件进行筛选
    if (true)
    {
        e->acceptProposedAction();	//放行，否则不会执行dropEvent()函数
    }
}

// 在拖动文件图标到ddl_block区域后被触发
void ddl_block::dropEvent(QDropEvent *e)
{
    QList<QUrl> urls = e->mimeData()->urls();
    if(urls.isEmpty())
        return ;
    foreach (QUrl u, urls)
    {
        QString filepath = u.toLocalFile();
        this->m_ddl->AddPath(filepath);
        WorkingFileListItem* tmp = new WorkingFileListItem(filepath);
        m_ListWidget->addItem(tmp);

        // 连接双击点击列表路径信号，和打开文件槽
        connect(m_ListWidget, &QListWidget::itemDoubleClicked, this, &ddl_block::slot_open);
    }
}

void ddl_block::SetWorkingFileSpace()
{
    this->m_FileWidget = new QWidget;       // 新建工作文件窗口
    m_FileWidget->setWindowTitle("工作文件");
    m_FileWidget->resize(500,500);

    this->m_ListWidget = new QListWidget(this->m_FileWidget);     // 新建工作文件列表
    m_ListWidget->resize(500,400);
    m_ListWidget->setFont(QFont("Hack",14));
    vector<WorkingFile> allFile = this->m_ddl->GetWorkingFile();

    // 显示工作文件路径
    for(auto it = allFile.begin(); it != allFile.end(); it++) {
        WorkingFileListItem* tmp = new WorkingFileListItem(it->GetFilePath());
        m_ListWidget->addItem(tmp);

        // 连接双击点击列表路径信号，和打开文件槽
        connect(m_ListWidget, &QListWidget::itemDoubleClicked, this, &ddl_block::slot_open);
//        connect(m_ListWidget, &QAction::triggered, this, &)
    }
}

//void ddl_block::OnClickedWorkingFileMenu()
//{
//    QAction *pEven = qobject_cast<QAction *>(this->sender());

//    //获取发送信息
//    int iType = pEven->data().toInt();
//    switch (iType)
//    {
//    case 1:
//        break;
//    case 2:
//        break;
//    case 3:
//        break;
//    default:
//        break;
//    }
//}

void ddl_block::ShowWorkingFileSpace()
{
    this->m_FileWidget->show();
}

void ddl_block::slot_open(QListWidgetItem *item)
{
    QString path = item->text();
    this->m_ddl->OpenFile(path);
}

void ddl_block::slot_voidToint(int rank)
{
    emit getInt(rank);
}

void ddl_block::emit_interchange()
{
    emit getInt(this->rank);
}

void ddl_block::emit_interchange_prev()
{
    emit getInt_prev(this->rank);
}


void ddl_block::emit_interchange_succ()
{
    emit getInt_succ(this->rank);
}



