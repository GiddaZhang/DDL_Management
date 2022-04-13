#include "button_next.h"
#include "QDebug"


button_next::button_next(QMainWindow *parent):button(parent)
{

}

void button_next::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        emit ddl_end();
        this->hide();
        //qDebug() << rank;
    }
}
