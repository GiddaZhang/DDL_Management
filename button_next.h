#ifndef BUTTON_NEXT_H
#define BUTTON_NEXT_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QMainWindow>
#include <QLabel>
#include "button.h"

class button_next : public button
{
    Q_OBJECT
public:
    explicit button_next(QMainWindow *parent = nullptr);
    void mousePressEvent(QMouseEvent *ev) override;

signals:
    void ddl_end();

};

#endif // BUTTON_NEXT_H
