#ifndef DESCRIPTION_H
#define DESCRIPTION_H
#include <QString>
#include <QDateTime>
#include <QTime>
#include <memory>
#include <vector>
using namespace  std;

class Description
{
public:
    Description(QString);//默认构造函数
    void Change(QString);//修改
    //常引用只读
    const QDateTime& TimeStamp;
    const QString& Note;

private:
    QDateTime m_timeStamp;
    QString m_note;
};

#endif // DESCRIPTION_H
