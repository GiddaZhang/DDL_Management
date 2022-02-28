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

