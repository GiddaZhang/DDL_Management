#ifndef DESCRIPTION_H
#define DESCRIPTION_H
#include <QString>
#include <QDateTime>
#include <QTime>
#include <memory>
#include <vector>
using namespace  std;

class Description{
public:
    Description(QString);                   // 默认构造函数，用当前时间
    Description(QString, const QDateTime&); // 其它构造函数，不用当前时间
    Description(const Description&);        // 拷贝构造函数
    void Change(const QString&);            // 修改
    Description& operator=(const Description& other)
    {
        if(&other == this) {
            return *this;
        }
        this->m_note = other.m_note;
        this->m_timeStamp = other.m_timeStamp;
        return *this;
    }

    //常引用只读
//    const QDateTime& TimeStamp;
//    const QString& Note;
    // 常引用只读有时候不好使
    QDateTime GetStamp();
    QString GetNote();

private:
    QDateTime m_timeStamp;
    QString m_note;
};

#endif // DESCRIPTION_H
