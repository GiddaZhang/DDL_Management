#ifndef BUTTON_NEW_H
#define BUTTON_NEW_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <QMainWindow>
#include "button.h"

class button_new : public button
{
    Q_OBJECT
public:
    explicit button_new(QMainWindow *parent = nullptr);
    void mousePressEvent(QMouseEvent *ev) override;

signals:
    void newddl();

};

#endif // BUTTON_NEW_H
