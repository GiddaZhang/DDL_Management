#ifndef BUTTON_H
#define BUTTON_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMainWindow>

class button : public QPushButton
{
    Q_OBJECT
public:
    explicit button(QMainWindow *parent = nullptr);
    button(QLabel *parent = nullptr);
    void setParameters(int x, int y, int Length, int width);
    int rank;
    int m_x;
    int m_y;
    int m_length;
    int m_width;

signals:

};

#endif // BUTTON_H
