#include "button_next.h"
#include "QDebug"


button_next::button_next(QLabel *parent):button(parent)
{

}

void button_next::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        emit next_ddl(this->rank);
        //qDebug() << rank;
    }
}
