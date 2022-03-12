#include "axispainter.h"

AxisPainter::AxisPainter(QMainWindow *parent) : QWidget(parent)
{
    m_StartPos.setX(100);
    m_StartPos.setY(100);
    m_Width = 512;
    m_Height = 512;
}

void AxisPainter::PaintAxis(QPainter* painter)
{
    // 设置原点在左下角
//    painter->setWindow(0,height(),width(),-height());

    QPen pen;
    pen.setColor(Qt::black);
    painter->setPen(pen);
    QPoint axisStartPoint;
    QPoint axisXEndPoint; // x 轴终点
    QPoint axisYEndPoint; // y 轴终点

    axisStartPoint.setX(m_StartPos.x());
    axisStartPoint.setY(m_StartPos.y() + m_Height);

    axisXEndPoint.setX(m_StartPos.x() + m_Width);
    axisXEndPoint.setY(m_StartPos.y() + m_Height);

    axisYEndPoint.setX(m_StartPos.x());
    axisYEndPoint.setY(m_StartPos.y());

    painter->drawLine(axisStartPoint, axisXEndPoint);
    painter->drawLine(axisStartPoint, axisYEndPoint);
}

void AxisPainter::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    this->PaintAxis(&painter);
}
