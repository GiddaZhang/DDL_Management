#include "button_next_succ.h"
#include "QDebug"


button_next_succ::button_next_succ(QMainWindow *parent):button(parent)
{

}

void button_next_succ::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        emit ddl_end_rank(this->rank);
        this->hide();
        //qDebug() << rank;
    }
}
