#include "ddl.h"

DDL::DDL(Date a, QString b):
    Date(a), descrip(b)
{}

bool DDL::operator < (const DDL& b)
{
    return Date::operator <(b);
}
