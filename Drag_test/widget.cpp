//#include "widget.h"
//#include "ui_widget.h"
//#include <qdebug.h>
//#include <QDragEnterEvent>
//#include <QDropEvent>
//#include <QMimeData>
//#include <QUrl>

//Widget::Widget(QWidget *parent) :
//    QWidget(parent),
//    ui(new Ui::Widget)
//{
//    ui->setupUi(this);
//    this->setAcceptDrops(true);

//}

//Widget::~Widget()
//{
//    delete ui;
//}

//void Widget::dragEnterEvent(QDragEnterEvent *ev)
//{
//    if(ev->mimeData()->hasUrls())
//    {
//        ev->accept();//接收拖动进入事件
//    }
//    //ev->ignore();//若忽略该事件，则不会发生之后的事件，拖放到此结束
//}

//void Widget::dropEvent(QDropEvent *ev)
//{
//    if(ev->mimeData()->hasUrls())
//    {
//        QList<QUrl> urls = ev->mimeData()->urls();

//        for(int i = 0; i<urls.size(); i++)
//        {
//            qDebug()<<urls.at(i).toLocalFile();
//        }
//    }
//}
