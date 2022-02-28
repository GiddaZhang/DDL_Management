#include "date.h"

Date::Date(int s, int mi, int h, int d, int mo, int y):
    m_sec(s), m_min(mi), m_hour(h), m_day(d), m_mon(mo), m_year(y)
{}

bool Date::operator < (const Date& b)   // 没想到更好的方法，打扰了
{
    if(this->m_year < b.m_year)
        return true;
    else if(this->m_year == b.m_year) {
        if(this->m_mon < b.m_mon)
            return true;
        else if(this->m_mon == b.m_mon) {
            if(this->m_day < b.m_day)
                return true;
            else if(this->m_day == b.m_day) {
                if(this->m_hour < b.m_hour)
                    return true;
                else if(this->m_hour == b.m_hour) {
                    if(this->m_min < b.m_min)
                        return true;
                    else if(this->m_min == b.m_min) {
                        if(this->m_sec < b.m_sec)
                            return true;
                    }
                }
            }
        }
    }
    return false;
}
