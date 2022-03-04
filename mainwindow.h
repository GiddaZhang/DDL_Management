#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "ddl_block.h"
#include "button.h"
#include "button_new.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    button_new *m_button;
    ddl_block *m_block;
    void keyPressEvent(QKeyEvent *event) override;

public slots:
    void create_ddl();

private:
    Ui::MainWindow *ui;
    int DDL_number;//记录当前有效的ddl的个数


};

#endif // MAINWINDOW_H
