#include "button_next_prev.h"
#include "QDebug"


button_next_prev::button_next_prev(QMainWindow *parent):button(parent)
{

}

void button_next_prev::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        emit ddl_end_rank(this->rank);
        this->hide();
        //qDebug() << rank;
    }
}
