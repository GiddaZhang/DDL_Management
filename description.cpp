#include "description.h"


Description::Description(QString description):
    TimeStamp(m_timeStamp), Note(m_note)
{
    m_note = description;
    m_timeStamp = QDateTime::currentDateTime();
}

Description::Description(QString description, QDateTime dateTime):
    TimeStamp(m_timeStamp), Note(m_note)
{
    m_note = description;
    m_timeStamp = dateTime;
}

void Description::Change(const QString& description){
    m_note = description;
}

