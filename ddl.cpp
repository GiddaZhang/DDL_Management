#include "ddl.h"

DDL::DDL(Date a, QString b, QString c = "None"):
    Date(a), m_descrip(b), m_type(c)
{}

bool DDL::operator < (const DDL& b)
{
    return Date::operator <(b);
}
