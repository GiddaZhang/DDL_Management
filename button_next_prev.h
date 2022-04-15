#ifndef BUTTON_NEXT_PREV_H
#define BUTTON_NEXT_PREV_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QMainWindow>
#include <QLabel>
#include "button.h"

class button_next_prev : public button
{
    Q_OBJECT
public:
    explicit button_next_prev(QMainWindow *parent = nullptr);
    void mousePressEvent(QMouseEvent *ev) override;
    int rank;

signals:
    void ddl_end_rank(int);

};

#endif // BUTTON_NEXT_H