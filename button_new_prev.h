#ifndef BUTTON_NEW_PREV_H
#define BUTTON_NEW_PREV_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QMainWindow>
#include "button.h"

class button_new_prev : public button
{
    Q_OBJECT
public:
    explicit button_new_prev(QMainWindow *parent = nullptr);
    void mousePressEvent(QMouseEvent *ev) override;
    int rank;

signals:
    void newddl_withrank(int);

};

#endif // BUTTON_NEW_H
