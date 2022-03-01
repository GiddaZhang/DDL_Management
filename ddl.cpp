#include "ddl.h"

DDL::DDL(QString name, QString time, QString des, QString dur, DDL* prev, DDL* next):
    Name(m_name),CompleteState(m_completeDegree), AllDescription(m_AllDescription),
    Duration(m_duration), Due(m_due)
{
    SetDDL(name, TO_BE_STARTED, time, des, dur, prev, next);
}

Result DDL::SetDDL(QString name, int degree, QString time, QString des, QString dur,
                 DDL* prev, DDL* next)
{
   if(SetName(name)==INVALID||SetCompleteDegree(degree)==INVALID
           ||SetDuration(dur)==INVALID||SetDue(time)==INVALID){
       return INVALID;
   }
    AddDescription(des);
    m_prev = prev;
    m_next = next;
}

Result DDL::SetName(const QString& name)
{
    auto Finder = [&name](shared_ptr<DDL> ptr)->bool{return (ptr->Name == name);};
    auto it = find_if(m_AllDDL.begin(), m_AllDDL.end(), Finder);
    if (it == m_AllDDL.end()) {
        m_name = name;
        return VALID;
    }else{
        return INVALID;
    }
}

Result DDL::SetCompleteDegree(const CompleteDegree& degree)
{
    if(degree == STARTED || degree == QUARTER || degree == HALF_WAY
       || degree == THREE_QUARTERS || degree == TO_BE_REFINED
       || degree == COMPLETED || degree == HANDED_IN){
        m_completDegree = degree;
        return VALID;
    }else{
        return INVALID;
    }

}

Result DDL::SetDuration(const QString& dur)
{
    m_duration = QTime::fromString(dur);
}

Result DDL::SetDue(const QString& due)
{
    m_due = QDateTime::fromString(time, "yyyy-MM-dd hh:mm:ss");
}

Result DDL::AddDescription(const QString& des)
{

}

Result DDL::DeleteDescription(const int& num)
{

}

Result DDL::ModifyDescription(const int& num, const QString& new_des)
{

}

Result DDL::SetPrev(DDL* prev)
{
    m_prev = prev;
}

Result DDL::SetNext(DDL* next)
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

bool DDL::operator > (const DDL& b)
{
    return this->m_due > b.Due;
}
