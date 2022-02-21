#include "date.h"

Date::Date(int s, int mi, int h, int d, int mo, int y):
    sec(s), min(mi), hour(h), day(d), mon(mo), year(y)
{}

bool operator <(const Date& a, const Date& b)
{
    if(a.year < b.year)
        return true;
    else if(a.year == b.year){
        if(a.mon < b.mon)
            return true;
        else if(a.mon == b.mon){
            if(a.day < b.day)
                return true;
            else if(a.day == b.day){
                if(a.hour < b.hour)
                    return true;
                else if(a.hour == b.hour){
                    if(a.min < b.min)
                        return true;
                    else if(a.min == b.min){
                        if(a.sec < b.sec)
                            return true;
                    }
                }
            }
        }
    }
    return false;
}
