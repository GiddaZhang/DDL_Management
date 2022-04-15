#ifndef BUTTON_NEXT_SUCC_H
#define BUTTON_NEXT_SUCC_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QMainWindow>
#include <QLabel>
#include "button.h"

class button_next_succ : public button
{
    Q_OBJECT
public:
    explicit button_next_succ(QMainWindow *parent = nullptr);
    void mousePressEvent(QMouseEvent *ev) override;
    int rank;

signals:
    void ddl_end_rank(int);

};

#endif // BUTTON_NEXT_H