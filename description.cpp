#include "description.h"


Description::Description(QString description)
{
    m_note = description;
    m_timeStamp = QDateTime::currentDateTime();
}

Description::Description(QString description, const QDateTime& dateTime)
{
    m_note = description;
    m_timeStamp = dateTime;
}

Description::Description(const Description& other)
{
    m_note = other.m_note;
    m_timeStamp = other.m_timeStamp;
}

void Description::Change(const QString& description){
    m_note = description;
}

QDateTime Description::GetStamp(){
    return this->m_timeStamp;
}

QString Description::GetNote(){
    return this->m_note;
}
