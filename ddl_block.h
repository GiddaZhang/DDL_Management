#ifndef DDL_BLOCK_H
#define DDL_BLOCK_H

#include <QObject>
#include <QWidget>
#include <QMainWindow>
#include <QGroupBox>
#include <QLabel>
#include <QKeyEvent>
#include <QDebug>
#include <QMouseEvent>
#include "ddl.h"


class ddl_block : public QLabel
{
    Q_OBJECT
public:
    explicit ddl_block(QMainWindow *parent = nullptr);
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *ev) override;
    void setddl(int degree, bool state, bool turn,
                QString time, QString des,
                QString dur,
                DDL* prev, DDL* next);

private:
    DDL m_ddl;


signals:
    void newddl();
};

#endif // DDL_BLOCK_H
