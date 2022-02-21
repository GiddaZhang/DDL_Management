#ifndef DDL_H
#define DDL_H

#include <QString>
#include "date.h"

class DDL : public Date
{
public:
    DDL(Date, QString);
private:
    QString descrip;
};

#endif // DDL_H
