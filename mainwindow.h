#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "ddl_block.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QLabel *m_button;
    ddl_block *m_block;
    void keyPressEvent(QKeyEvent *event) override;
    void create_ddl();

private:
    Ui::MainWindow *ui;
    int DDL_number;//记录当前有效的ddl的个数
};

#endif // MAINWINDOW_H
