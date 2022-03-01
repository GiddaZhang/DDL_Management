#include "description.h"


Description::Description(QString description):TimeStamp(m_timeStamp), Note(m_note){
    m_note = description;
    m_timeStamp = QDateTime::currentDateTime();
}

void Description::Change(const QString& description){
    m_note = description;
}

