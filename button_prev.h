#ifndef BUTTON_PREV_H
#define BUTTON_PREV_H


#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QMainWindow>
#include <QLabel>
#include "button.h"

class button_prev : public button
{
    Q_OBJECT
public:
    explicit button_prev(QLabel *parent = nullptr);
    void mousePressEvent(QMouseEvent *ev) override;

signals:
    void prev_ddl(int rank);

};

#endif // BUTTON_PREV_H
