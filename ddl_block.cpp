#include "ddl_block.h"

ddl_block::ddl_block(QMainWindow *parent) : QLabel(parent)
{

}

void ddl_block::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W)
    {
        emit newddl();
        this->setGeometry(0, 0, 100, 100);
        qDebug() <<  "shit!!!";
    }
}

void ddl_block::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        emit newddl();
        this->setGeometry(0, 0, 100, 100);
        qDebug() <<  "shit!!!";
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
    delete this;
}

