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
#include "button_delete.h"
#include "button_new.h"
#include "button_next.h"


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
    int rank;//记录其在数组中的序号
    int m_x;
    int m_y;
    int m_length;
    int m_width;//用于存储每个ddl的位置及大小参数
    void setParameters(int x, int y, int Length, int width);
    virtual ~ddl_block();
    button_delete *Button_delete;
    button_next *Button_next;
    DDL *m_ddl;


private:



signals:
    void deleteddl();
    void show_tasks();
public slots:
    void slot_delete();
    void slot_tasks();

};

#endif // DDL_BLOCK_H
