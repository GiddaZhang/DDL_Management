#ifndef DDL_H
#define DDL_H

#include <QString>
#include "date.h"

class DDL : public Date
{
public:
    DDL(Date, QString, QString);
    bool operator < (const DDL&);   // 比较时间的先后用于排序："<"——先发生
private:
    QString descrip;    // 关于DDL的描述性文字
    QString type;       // DDL种类如作业、Pre等等，后续设计界面时做不同颜色显示
};

#endif // DDL_H
