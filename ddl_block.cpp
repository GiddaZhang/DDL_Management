#include "ddl_block.h"

// 使用ddl基类默认构造函数的派生类构造函数
ddl_block::ddl_block(QWidget *parent) : QLabel(parent), DDL(){
    m_pMenu = new QMenu(parent);// 初始化菜单指针，类型为pointer to QMenu

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
        connect(m_act[i], SIGNAL(triggered()), this, SLOT(OnClickedPopMenu())); // 连接鼠标点击信号
    }

    setAcceptDrops(true);                // 允许将文档拖动到窗口中
    SetWorkingFileSpace();               // 初始化工作文件窗口
    SetNoteSpace();                      // 初始化留言窗口
    SetColor();                          // 随机一个颜色

    setMouseTracking(true);
    connect(this, SIGNAL(mouseMove()), this, SLOT(show_timelim()));
}

// 使用ddl基类拷贝构造函数的派生类构造函数
ddl_block::ddl_block(DDL& ddl, QWidget *parent)
: QLabel(parent), DDL(ddl){
    m_pMenu = new QMenu(parent);// 初始化菜单指针，类型为pointer to QMenu

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
        connect(m_act[i], SIGNAL(triggered()), this, SLOT(OnClickedPopMenu())); // 连接鼠标点击信号
    }

    setAcceptDrops(true);                // 允许将文档拖动到窗口中
    SetWorkingFileSpace();               // 初始化工作文件窗口
    SetNoteSpace();                      // 初始化留言窗口
    SetColor();                          // 随机一个颜色

    setMouseTracking(true);
    connect(this, SIGNAL(mouseMove()), this, SLOT(show_timelim()));
}

// 右键单击发射show_tasks信号
void ddl_block::mousePressEvent(QMouseEvent *ev){
    if(ev->button() == Qt::RightButton){
        emit show_tasks();
    }
}

void ddl_block::mouseMoveEvent(QMouseEvent *ev){
    emit mouseMove();
}

// 设置模块形状参数
void ddl_block::setParameters(int x, int y, int Length, int width){
    this->m_x = x;
    this->m_y = y;
    this->m_length = Length;
    this->m_width = width;
}

// 析构函数
ddl_block::~ddl_block(){

}

void ddl_block::slot_delete(){

}

void ddl_block::slot_tasks(){
    //qDebug() << "Cyka";
}

// 根据菜单中的点击事件启动菜单栏对应的任务
void ddl_block::OnClickedPopMenu(){
    QAction *pEven = qobject_cast<QAction *>(this->sender());

    //获取发送的信息
    int iType = pEven->data().toInt();
    switch (iType){
    case 1:
        this->ShowWorkingFileSpace();// 显示工作文件路径与写入、删除路径
        break;
    case 2:
        emit_interchange();// 对应删除操作
        break;
    case 3:
        this->ShowNoteSpace();// 对应留言操作
        break;
    case 4:
        emit_interchange_succ();// 对应设置后继操作
        break;
    case 5:
        emit_interchange_prev();// 对应设置前驱操作
        break;
    default:
        break;
    }
}

//对拖放事件进行筛选
void ddl_block::dragEnterEvent(QDragEnterEvent *e){
    if (true){
        e->acceptProposedAction();	//放行，否则不会执行dropEvent()函数
    }
}

// 在拖动文件图标到ddl_block区域后被触发
void ddl_block::dropEvent(QDropEvent *e){
    QList<QUrl> urls = e->mimeData()->urls();

    // 判断early exit条件
    if(urls.isEmpty()){
        return ;
    }
    foreach (QUrl u, urls){
        QString filepath = u.toLocalFile();

        // 更新动态变量：更新工作文件路径
        this->AddPath(filepath);

        // 更新静态变量：更新工作文件路径
        shared_ptr<DDL> thisBlock = DDL::GetDDLPtr(this->GetName());
        thisBlock->AddPath(filepath);

        // 将工作文件路径放入显示窗口中
        WorkingFileListItem* tmp = new WorkingFileListItem(filepath);
        m_ListWidget->addItem(tmp);

        // 连接双击点击列表路径信号，和打开文件槽
        connect(m_ListWidget, &QListWidget::itemDoubleClicked, this, &ddl_block::slot_openFile);
    }
}

// 设置工作文件窗口
void ddl_block::SetWorkingFileSpace(){
    m_FileWidget = new QWidget;                         // 新建工作文件窗口
    m_FileWidget->setWindowTitle("工作文件");            // 设置窗口名称
    m_FileWidget->resize(500, 500);                     // 设置大小
    m_FileWidget->setObjectName("FileWidget");          // 设置窗口名称

    m_ListWidget = new QListWidget(m_FileWidget);       // 新建工作文件列表，将其放入工作文件窗口内
    m_ListWidget->resize(500,450);                      // 设置大小
    m_ListWidget->setFont(QFont("Consolas, STZhongsong",14));        // 设置字体
    m_ListWidget->setProperty("contextMenuPolicy", Qt::CustomContextMenu);

    // 设置工作文件菜单
    m_pMenu_workingFile = new QMenu(m_FileWidget);
    m_act_workingFile[0] = new QAction("删除", this);
    m_act_workingFile[1] = new QAction("备份", this);
    m_act_workingFile[2] = new QAction("打开", this);
    m_pMenu_workingFile->addAction(m_act_workingFile[2]);
    m_pMenu_workingFile->addAction(m_act_workingFile[0]);
    m_pMenu_workingFile->addAction(m_act_workingFile[1]);

    // 删除
    connect(this->m_act_workingFile[0], SIGNAL(triggered()), this, SLOT(slot_deleteFile()));
    // 备份
    connect(this->m_act_workingFile[1], SIGNAL(triggered()), this, SLOT(slot_saveFile()));
    // 打开
    connect(this->m_act_workingFile[2], SIGNAL(triggered()), this, SLOT(slot_openFileMenu()));
    // 弹出菜单
    connect(this->m_ListWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
        this, SLOT(OnWorkingFileMenu(const QPoint &)));

    // 显示工作文件路径
    for(auto it = (this->m_allFilePath).begin(); it != (this->m_allFilePath).end(); it++) {
        WorkingFileListItem* tmp = new WorkingFileListItem(it->GetFilePath());
        m_ListWidget->addItem(tmp);
        // 连接双击点击列表路径信号，和打开文件槽
        connect(m_ListWidget, &QListWidget::itemDoubleClicked, this, &ddl_block::slot_openFile);
    }
    // 设置一键打开按钮
    QPushButton* openAllFileButton = new QPushButton(m_FileWidget);
    openAllFileButton->setText("Open All");
    openAllFileButton->setFont(QFont("Consolas", 12));
    openAllFileButton->resize(100,30);
    openAllFileButton->move(100, 465);

    // 连接
    connect(openAllFileButton, &QPushButton::pressed, this, &ddl_block::slot_openAll);

    // 设置一键存档按钮
    QPushButton* saveAllFileButton = new QPushButton(m_FileWidget);
    saveAllFileButton->setText("Save All");
    saveAllFileButton->setFont(QFont("Consolas", 12));
    saveAllFileButton->resize(100,30);
    saveAllFileButton->move(300, 465);

    // 连接
    connect(saveAllFileButton, &QPushButton::pressed, this, &ddl_block::slot_saveAll);
}

// 显示工作文件窗口
void ddl_block::ShowWorkingFileSpace(){
    m_FileWidget->show();
}

// 设置留言界面
void ddl_block::SetNoteSpace(){
    m_NoteWidget = new QWidget;                         // 新建留言窗口
    m_NoteWidget->setWindowTitle("留言");                // 设置窗口名称
    m_NoteWidget->resize(500,500);                      // 设置大小

    m_NoteListWidget = new QListWidget(m_NoteWidget);       // 新建留言列表，将其放入留言窗口内
    m_NoteListWidget->resize(500,450);                      // 设置大小
    m_NoteListWidget->setFont(QFont("Consolas, STZhongsong",14));            // 设置字体

    // 显示留言
    for(auto it = (this->m_allDescription).begin(); it != (this->m_allDescription).end(); it++) {
        NoteListItem* tmp = new NoteListItem((it->GetStamp()).toString()+" "+it->GetNote());
        m_NoteListWidget->addItem(tmp);
        // 连接双击点击留言信号，和删除留言槽
        connect(m_NoteListWidget, &QListWidget::itemDoubleClicked, this, &ddl_block::slot_deleteNote);
    }

    // 设置新建留言按钮
    QPushButton* newNoteButton = new QPushButton(m_NoteWidget);
    newNoteButton->setText("new Note");
    newNoteButton->setFont(QFont("Consolas", 12));
    newNoteButton->resize(100,30);
    newNoteButton->move(200, 465);

    // 连接
    connect(newNoteButton, &QPushButton::pressed, this, &ddl_block::slot_addNote);
}

// 显示留言界面
void ddl_block::ShowNoteSpace(){
    m_NoteWidget->show();
}

// 设置block模块颜色
void ddl_block::SetColor(){
    int random_num = rand() % 5 + 1;
    QString style = "ddl_block" + QString::number(random_num);
    this->setObjectName(style);    // 设置名称让Qss匹配，随机一个颜色
}

// 打开单个文档
void ddl_block::slot_openFile(QListWidgetItem *item){
    OpenFile(item->text());
}

// 打开所有文档
void ddl_block::slot_openAll()
{
    OpenAllFile();
}

// 删除特定文档
void ddl_block::slot_deleteFile()
{
    QList<QListWidgetItem*> items = this->m_ListWidget->selectedItems();
    foreach(QListWidgetItem* var, items) {
        // 这个东西工作原理不明，不过var就是待删除的item
        this->m_ListWidget->removeItemWidget(var);
        items.removeOne(var);

        // 更新动态变量：更新工作文件路径
        QString path = var->text();
        this->DeletePath(path);

        // 更新静态变量：更新工作文件路径
        shared_ptr<DDL> thisBlock = DDL::GetDDLPtr(this->GetName());
        thisBlock->DeletePath(path);
        delete var;
    }
}

void ddl_block::slot_openFileMenu()
{
    QList<QListWidgetItem*> items = this->m_ListWidget->selectedItems();
    foreach(QListWidgetItem* var, items) {
        // 这个东西工作原理不明，不过var就是待打开的item
        OpenFile(var->text());
    }
}

void ddl_block::slot_saveFile()
{
    QList<QListWidgetItem*> items = this->m_ListWidget->selectedItems();
    foreach(QListWidgetItem* var, items) {
        // 这个东西工作原理不明，不过var就是待save的item
        QString path = var->text();
        WorkingFile* tmp = new WorkingFile(path);
        tmp->SaveToFolder(path);
        delete tmp;
    }
}

void ddl_block::OnWorkingFileMenu(const QPoint &pos)
{
    m_pMenu_workingFile->exec(QCursor::pos());
}

// 拷贝所有文档
void ddl_block::slot_saveAll(){
    // 遍历所有文件路径
    QString path;
    for (int j = 0; j < this->m_ListWidget->count(); j++) {
        path = this->m_ListWidget->item(j)->text();
        WorkingFile* tmp = new WorkingFile(path);
        tmp->SaveToFolder(path);
        delete tmp;
    }
}

void ddl_block::slot_addNote(){
    // 获取留言输入
    QInputDialog type_in_note(this);
    QString tmp_note = type_in_note.getText(this, "note", "please type in new Note", QLineEdit::Normal);

    // 更新动态变量：更新留言
    this->AddDescription(tmp_note);

    // 更新静态变量：更新留言
    shared_ptr<DDL> thisBlock = DDL::GetDDLPtr(this->GetName());
    thisBlock->AddDescription(tmp_note);

    // 整合留言和时间戳
    QString integrated_Note = (QDateTime::currentDateTime()).toString()+" "+tmp_note;

    // 将整合后的留言放入显示窗口中
    NoteListItem* tmp = new NoteListItem(integrated_Note);
    m_NoteListWidget->addItem(tmp);

    // 连接双击点击留言信号，和删除留言槽
    connect(m_NoteListWidget, &QListWidget::itemDoubleClicked, this, &ddl_block::slot_deleteNote);
}

void ddl_block::slot_deleteNote(){
    QList<QListWidgetItem*> items = this->m_NoteListWidget->selectedItems();
    foreach(QListWidgetItem* var, items) {
        // 这个东西工作原理不明，不过var就是待删除的item
        this->m_ListWidget->removeItemWidget(var);
        items.removeOne(var);

        // 更新动态变量：更新工作文件路径
        QString content = var->text();
        // 此时留言的内容要去掉前面的日期。
        QStringList list = content.split(" ");
        QString note = list[list.size() - 1];
        this->DeleteDescription(note);

        // 更新静态变量：更新工作文件路径
        shared_ptr<DDL> thisBlock = DDL::GetDDLPtr(this->GetName());
        thisBlock->DeleteDescription(note);
        delete var;
    }
}

void ddl_block::show_timelim(){
    QToolTip::showText(QCursor().pos(), "起始：" + (this->GetComm()).toString() + "\n截止：" + (this->GetDue()).toString());
}

// 一些发射信号的函数
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
