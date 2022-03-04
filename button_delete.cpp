#include "button_delete.h"

button_delete::button_delete(QLabel *parent) : button(parent)
{

}

void button_delete::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        emit delete_ddl();
    }
}




