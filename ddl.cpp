#include "ddl.h"

DDL::DDL(int degree, bool state, bool turn,
         QString time, QString des, QString dur,
         DDL* prev, DDL* next):
    QDateTime(QDateTime::fromString(time, "yyyy-MM-dd hh:mm:ss")),
    m_completDegree(degree), m_completState(state),
    m_turnInState(turn), m_descrip(des),
    m_duration(QTime::fromString(dur)),
    m_prev(prev), m_next(next)
{}

bool DDL::SetCompletDeg(int degree)
{
    m_completDegree = degree;
    return true;
}

bool DDL::SetCompletState(bool state)
{
    m_completState = state;
    return true;
}

bool DDL::SetTurnInState(bool turn)
{
    m_turnInState = turn;
    return true;
}

bool DDL::SetDescirp(const QString& des)
{
    m_descrip = des;
    return true;
}

bool DDL::SetDuration(const QString& dur)
{
    m_duration = QTime::fromString(dur);
    return true;
}

bool DDL::SetPrev(DDL* prev)
{
    m_prev = prev;
    return true;
}

bool DDL::SetNext(DDL* next)
{
    m_next = next;
    return true;
}

double DDL::GetUrgency()
{
    qint64 duration_sec = m_duration.hour() * 3600
            + m_duration.minute() * 60
            + m_duration.second();                                  // 预计用时，单位秒
    qint64 cur_sec = QDateTime::currentDateTime().secsTo(*this);    // 目前剩余时间，单位秒
    return ((double) duration_sec / (double)cur_sec) * 100;
}

DDL* DDL::GetPrev()
{
    return m_prev;
}

DDL* DDL::GetNext()
{
    return m_next;
}

bool DDL::operator < (const DDL& b)
{
    return QDateTime::operator <(b);
}
