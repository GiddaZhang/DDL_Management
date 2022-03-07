#include "button.h"

button::button(QMainWindow *parent) : QPushButton(parent)
{

}

button::button(QLabel *parent):QPushButton(parent)
{

}

void button::setParameters(int x, int y, int Length, int width)
{
    this->m_x = x;
    this->m_y = y;
    this->m_length = Length;
    this->m_width = width;
}
