#ifndef DDL_H
#define DDL_H

#include <QString>
#include "date.h"

class DDL : public Date
{
public:
    DDL(Date, QString);
    bool operator < (const DDL&);   // 比较时间的先后："<"——先发生
private:
    QString descrip;
};

#endif // DDL_H
