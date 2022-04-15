#include "button_new_prev.h"
#include <QDebug>

button_new_prev::button_new_prev(QMainWindow *parent) : button(parent)
{

}

void button_new_prev::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        this->hide();
        emit newddl_withrank(this->rank);
        //ok
    }
}
