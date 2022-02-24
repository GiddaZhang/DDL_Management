#include "ddl.h"

DDL::DDL(int degree, bool state, bool turn,
         QString time, QString des, QString dur,
         DDL* prev, DDL* next):
    CompletDegree(m_completDegree), CompletState(m_completState),
    TurnInState(m_turnInState), Descrip(m_descrip),
    Duration(m_duration), Due(m_due)
{
    SetDDL(degree, state, turn, time, des, dur, prev, next);
}

void DDL::SetDDL(int degree, bool state, bool turn,
                 QString time, QString des, QString dur,
                 DDL* prev, DDL* next)
{
    m_completDegree = degree;
    m_completState = state;
    m_turnInState = turn;
    m_due = QDateTime::fromString(time, "yyyy-MM-dd hh:mm:ss");
    m_descrip = des;
    m_duration = QTime::fromString(dur);
    m_prev = prev;
    m_next = next;
}

void DDL::SetCompletDeg(int degree)
{
    m_completDegree = degree;
}

void DDL::SetCompletState(bool state)
{
    m_completState = state;
}

void DDL::SetTurnInState(bool turn)
{
    m_turnInState = turn;
}

void DDL::SetDescirp(const QString& des)
{
    m_descrip = des;
}

void DDL::SetDuration(const QString& dur)
{
    m_duration = QTime::fromString(dur);
}

void DDL::SetPrev(DDL* prev)
{
    m_prev = prev;
}

void DDL::SetNext(DDL* next)
{
    m_next = next;
}

double DDL::GetUrgency()
{
    qint64 duration_sec = m_duration.hour() * 3600
            + m_duration.minute() * 60
            + m_duration.second();                                  // 预计用时，单位秒
    qint64 cur_sec = QDateTime::currentDateTime().secsTo(this->Due);    // 目前剩余时间，单位秒
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
    return this->m_due < b.Due;
}
