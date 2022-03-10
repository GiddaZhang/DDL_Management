#include "button_prev.h"
#include "QDebug"


button_prev::button_prev(QLabel *parent):button(parent)
{

}

void button_prev::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        emit prev_ddl(this->rank);
        //qDebug() << rank;
    }
}
