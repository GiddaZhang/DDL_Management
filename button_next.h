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
    explicit button_next(QLabel *parent = nullptr);
    void mousePressEvent(QMouseEvent *ev) override;

signals:
    void next_ddl(int rank);

};

#endif // BUTTON_NEXT_H
