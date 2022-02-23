#include "ddl.h"

DDL::DDL(QString a, QString b, QString dur, QString c = "None"):
    QDateTime(QDateTime::fromString(a, "yyyy-MM-dd hh:mm:ss")), m_descrip(b), m_type(c),
    m_duration(QTime::fromString(dur))
{}

double DDL::GetUrgency()
{
    qint64 duration_sec = m_duration.hour() * 3600
            + m_duration.minute() * 60
            + m_duration.second();                                  // 预计用时，单位秒
    qint64 cur_sec = QDateTime::currentDateTime().secsTo(*this);    // 目前剩余时间，单位秒
    return ((double) duration_sec / (double)cur_sec) * 100;
}

bool DDL::operator < (const DDL& b)
{
    return QDateTime::operator <(b);
}
