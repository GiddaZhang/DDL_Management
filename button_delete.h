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
    int rank;


signals:
    void delete_ddl(int rank);
    void deleteddl();

};

#endif // BUTTON_DELETE_H
