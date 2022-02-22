#ifndef DATE_H
#define DATE_H


class Date
{
public:
    Date(int, int, int, int, int, int);
    bool operator < (const Date&);   // 比较时间的先后："<"——先发生
private:
    int sec;   // 秒，正常范围从 0 到 59，但允许至 61
    int min;   // 分，范围从 0 到 59
    int hour;  // 小时，范围从 0 到 23
    int day;   // 天，范围从 1 到 31
    int mon;   // 月，范围从 1 到 12
    int year;  // 年
};

#endif // DATE_H
