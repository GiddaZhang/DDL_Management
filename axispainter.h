#ifndef AXISPAINTER_H
#define AXISPAINTER_H

#include <QWidget>
#include <QPainter>
#include <QMainWindow>

//————————————————
//版权声明：本文为CSDN博主「云胡同学」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
//原文链接：https://blog.csdn.net/qq_32046111/article/details/122449362

class AxisPainter: public QWidget
{
public:
    AxisPainter(QMainWindow *parent = nullptr);
    void PaintAxis(QPainter* painter);

protected:
    void paintEvent(QPaintEvent *e);

private:
    QPoint m_StartPos; // 起始点
    int m_Width;       // 坐标轴的宽度
    int m_Height;      // 坐标轴的高度
};

#endif // AXISPAINTER_H
