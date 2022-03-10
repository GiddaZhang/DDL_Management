#include "ddl_block.h"

ddl_block::ddl_block(QMainWindow *parent) : QLabel(parent)
{
    this->m_ddl = new DDL;      // 初始化DDL

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

//    QList<QAction*> list = m_pMenu->actions();
//    foreach (QAction* pAction, list)
//    delete pAction;
//    delete m_pMenu;
//    释放内存会出问题
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
    qDebug() << "Cyka";
}

void ddl_block::OnClickedPopMenu()
{
    QAction *pEven = qobject_cast<QAction *>(this->sender());

    //获取发送信息
    int iType = pEven->data().toInt();
    switch (iType)
    {
    case 1:
        QMessageBox::about(this, "工作", this->m_ddl->GetName());
        break;
    case 2:
//        QMessageBox::about(this, "删除", pEven->text());
        break;
    case 3:
        QMessageBox::about(this, "留言", pEven->text());
        break;
    case 4:
        QMessageBox::about(this, "添加后继", pEven->text());
        break;
    case 5:
        QMessageBox::about(this, "添加前驱", pEven->text());
        break;
    default:
        break;
    }
}





