#ifndef DDL_H
#define DDL_H

#include <QString>
#include <QDateTime>
#include <QTime>

class DDL : public QDateTime
{
public:
    DDL(int degree = 100, bool state = false, bool turn = false,
        QString time = "2022-03-24 12:24:36", QString des = "Qt",
        QString dur = "12:00:00",
        DDL* prev = NULL, DDL* next = NULL);

    bool SetCompletDeg(int);          // 设置任务完成度，返回设置是否成功
    bool SetCompletState(bool);       // 设置任务完成状态，返回设置是否成功
    bool SetTurnInState(bool);        // 设置任务提交状态，返回设置是否成功
    bool SetDescirp(const QString&);  // 设置DDL的描述性文字，返回设置是否成功
    bool SetDuration(const QString&); // 设置任务持续时间，返回设置是否成功
    bool SetPrev(DDL*);               // 设置前驱任务，返回设置是否成功
    bool SetNext(DDL*);               // 设置后继任务，返回设置是否成功

    double GetUrgency();              // 返回DDL紧迫程度，预期耗时/(DDL时间-当前时间)*100%
    DDL* GetPrev();                   // 返回前驱任务
    DDL* GetNext();                   // 返回后继任务
    bool operator < (const DDL&);     // 比较时间的先后用于排序："<"——先发生

private:
    int m_completDegree;    // 任务完成度，取值0~100
    bool m_completState;    // 任务完成状态。即使完成度100，也会因未提交为false
    bool m_turnInState;     // 任务提交状态
    QString m_descrip;      // 关于DDL的描述性文字
    QTime m_duration;       // 任务持续时间
    DDL* m_prev;            // 前驱任务
    DDL* m_next;            // 后继任务
// 说明：任务结束时间在基类QDateTime中，任务紧迫度通过接口GetUrgency()间接计算，不用属性进行维护
};

#endif // DDL_H
