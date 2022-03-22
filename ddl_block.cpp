#include "ddl_block.h"

ddl_block::ddl_block(QMainWindow *parent) : QLabel(parent), DDL(){
    // 用parent初始化QLabel基类，DDL基类只调用默认构造函数

    // 初始化菜单指针，类型为pointer to QMenu
    m_pMenu = new QMenu(parent);

    // 初始化菜单栏指针，类型为pointer to QAction
    m_act[0] = new QAction("工作", this);
    m_act[1] = new QAction("删除", this);
    m_act[2] = new QAction("留言", this);
    m_act[3] = new QAction("添加后继", this);
    m_act[4] = new QAction("添加前驱", this);

    // 设置五个菜单栏
    for(int i = 0; i < 5; i++) {
        m_pMenu->addAction(m_act[i]);    // 将菜单栏添加到菜单上
        m_act[i]->setData(i + 1);        //设置菜单栏发送的数据（int型），它们会在槽OnClickedPopMenu()中与具体行为产生联系
        connect(m_act[i], SIGNAL(triggered()), this, SLOT(OnClickedPopMenu())); // 连接鼠标右键点击信号
    }

    setAcceptDrops(true);                // 允许将文档拖动到窗口中
    SetWorkingFileSpace();               // 初始化工作文件窗口
}

void ddl_block::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::RightButton)
    {
        emit show_tasks();
    }
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

void ddl_block::OnClickedPopMenu(){
    QAction *pEven = qobject_cast<QAction *>(this->sender());

    //获取发送信息
    int iType = pEven->data().toInt();
    switch (iType){
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

void ddl_block::dragEnterEvent(QDragEnterEvent *e){
    //对拖放事件进行筛选
    if (true){
        e->acceptProposedAction();	//放行，否则不会执行dropEvent()函数
    }
}

// 在拖动文件图标到ddl_block区域后被触发
void ddl_block::dropEvent(QDropEvent *e){
    QList<QUrl> urls = e->mimeData()->urls();
    if(urls.isEmpty()){
        return ;
    }
    foreach (QUrl u, urls){
        QString filepath = u.toLocalFile();
        this->AddPath(filepath);
        WorkingFileListItem* tmp = new WorkingFileListItem(filepath);
        m_ListWidget->addItem(tmp);
        // delete tmp;
        // 连接双击点击列表路径信号，和打开文件槽
        connect(m_ListWidget, &QListWidget::itemDoubleClicked, this, &ddl_block::slot_open);
    }
}

void ddl_block::SetWorkingFileSpace(){
    m_FileWidget = new QWidget;                         // 新建工作文件窗口
    m_FileWidget->setWindowTitle("工作文件");            // 设置窗口名称
    m_FileWidget->resize(500,500);                      // 设置大小

    m_ListWidget = new QListWidget(m_FileWidget);       // 新建工作文件列表，将其放入工作文件窗口内
    m_ListWidget->resize(500,400);                      // 设置大小
    m_ListWidget->setFont(QFont("Hack",14));            // 设置字体

    // 显示工作文件路径
    for(auto it = (this->m_allFilePath).begin(); it != (this->m_allFilePath).end(); it++) {
        WorkingFileListItem* tmp = new WorkingFileListItem(it->GetFilePath());
        m_ListWidget->addItem(tmp);
        // delete tmp;
        // 连接双击点击列表路径信号，和打开文件槽
        connect(m_ListWidget, &QListWidget::itemDoubleClicked, this, &ddl_block::slot_open);
    }
    // 设置一键打开按钮
    QPushButton* openAllFileButton = new QPushButton(m_FileWidget);
    openAllFileButton->setText("Open All");
    openAllFileButton->setFont(QFont("Hack", 12));
    openAllFileButton->resize(100,30);
    openAllFileButton->move(200, 430);

    // 连接
    connect(openAllFileButton, &QPushButton::pressed, this, &ddl_block::slot_openAll);

    // 设置一键存档按钮
    QPushButton* saveAllFileButton = new QPushButton(m_FileWidget);
    saveAllFileButton->setText("Save All");
    saveAllFileButton->setFont(QFont("Hack", 12));
    saveAllFileButton->resize(100,30);
    saveAllFileButton->move(200, 465);

    // 连接
    connect(saveAllFileButton, &QPushButton::pressed, this, &ddl_block::slot_saveAll);
}

void ddl_block::ShowWorkingFileSpace(){
    m_FileWidget->show();
}

void ddl_block::slot_open(QListWidgetItem *item){
    OpenFile(item->text());
}

void ddl_block::slot_openAll(){
    OpenAllFile();
}

void ddl_block::slot_saveAll(){
    // 遍历所有文件路径
    QString path;
    for (int j = 0; j < m_ListWidget->count(); j++) {
        WorkingFile* tmp = new WorkingFile(m_ListWidget->item(j)->text());
        tmp->SaveToFolder(path);
        delete tmp;
        }
}

void ddl_block::slot_voidToint(int rank){
    emit getInt(rank);
}

void ddl_block::emit_interchange(){
    emit getInt(rank);
}

void ddl_block::emit_interchange_prev(){
    emit getInt_prev(rank);
}

void ddl_block::emit_interchange_succ(){
    emit getInt_succ(rank);
}
