#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDragEnterEvent>
#include <QFile>
#include <QDebug>
#include <QMimeData>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->textEdit->setAcceptDrops(false); //禁用控件的放下操作
    this->setAcceptDrops(true);//启用放下操作
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    //对拖放事件进行筛选
    if (true)
    {
        e->acceptProposedAction();	//放行，否则不会执行dropEvent()函数
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{
    QList<QUrl> urls = e->mimeData()->urls();
    if(urls.isEmpty())
        return ;
    qDebug()<< urls.size();
    foreach (QUrl u, urls)
    {
        QString filepath = u.toLocalFile();
        pathlist.append(filepath);
    }
    //去掉重复路径
    pathlist = pathlist.toSet().toList();
    ui->textEdit->clear();
    for(int i=0;i<pathlist.size();++i)
    {
        QString path = pathlist.at(i);
        ui->textEdit->append(path);
    }
}



void MainWindow::on_pushButton_upload_clicked()
{
    qDebug()<<pathlist;
}
