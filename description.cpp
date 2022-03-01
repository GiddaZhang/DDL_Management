#include "description.h"


Description::Description(QString des)
{
    m_note = des;
    m_timeStamp = QDateTime::currentDateTime();
}

void Description::Change(QString)
{
    m_note = des;
}

