#include "ddl.h"

DDL::DDL(QString a, QString b, QString c = "None"):
    QDateTime(QDateTime::fromString(a, "yyyy-MM-dd hh:mm:ss")), m_descrip(b), m_type(c)
{}

bool DDL::operator < (const DDL& b)
{
    return QDateTime::operator <(b);
}
