#ifndef BUTTON_DELETE_H
#define BUTTON_DELETE_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QMainWindow>
#include <QLabel>
#include "button.h"

class button_delete : public button
{
    Q_OBJECT
public:
    explicit button_delete(QLabel *parent = nullptr);
    void mousePressEvent(QMouseEvent *ev) override;
    void setParameters(int x, int y, int Length, int width);
    int rank;
    int m_x;
    int m_y;
    int m_length;
    int m_width;


signals:
    void delete_ddl(int rank);

};

#endif // BUTTON_DELETE_H
