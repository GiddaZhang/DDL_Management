#include "button_new_next.h"
#include <QDebug>

button_new_next::button_new_next(QMainWindow *parent) : button(parent)
{

}

void button_new_next::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        this->hide();
        emit newddl_withrank(this->rank);
        //ok
    }
}
