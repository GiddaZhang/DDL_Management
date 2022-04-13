#include "button_new.h"
#include <QDebug>

button_new::button_new(QMainWindow *parent) : button(parent)
{

}

void button_new::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        this->hide();
        emit newddl();
    }
}
