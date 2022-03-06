#include "button_delete.h"
#include <QDebug>
button_delete::button_delete(QLabel *parent) : button(parent)
{

}

void button_delete::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        emit delete_ddl(this->rank);
    }
}

void button_delete::setParameters(int x, int y, int Length, int width)
{
    this->m_x = x;
    this->m_y = y;
    this->m_length = Length;
    this->m_width = width;
}




