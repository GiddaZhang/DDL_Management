#include "date.h"

Date::Date(int s, int mi, int h, int d, int mo, int y):
    sec(s), min(mi), hour(h), day(d), mon(mo), year(y)
{}

bool Date::operator < (const Date& b)
{
    if(this->year < b.year)
        return true;
    else if(this->year == b.year){
        if(this->mon < b.mon)
            return true;
        else if(this->mon == b.mon){
            if(this->day < b.day)
                return true;
            else if(this->day == b.day){
                if(this->hour < b.hour)
                    return true;
                else if(this->hour == b.hour){
                    if(this->min < b.min)
                        return true;
                    else if(this->min == b.min){
                        if(this->sec < b.sec)
                            return true;
                    }
                }
            }
        }
    }
    return false;
}
