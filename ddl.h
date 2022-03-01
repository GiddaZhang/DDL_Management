#ifndef DDL_H
#define DDL_H

#include <QString>
#include <QDateTime>
#include <QTime>
#include <memory>
#include <vector>
#include <description.h>
using namespace  std;

//完成情况，为用户提供几个可选选项
enum CompleteDegree{
    TO_BE_STARTED,STARTED,QUARTER,HALF_WAY,THREE_QUARTERS,TO_BE_REFINED,COMPLETED,HANDED_IN
};

//修改操作是否合理
enum Result{
    VALID,INVALID
};

class DDL
{
public:
    DDL(QString name = "UNKNOWN", QString commence = "2022-03-24 12:24:36",
        QString due = "2022-03-24 12:24:36", QString des,
        QString dur = "12:00:00", DDL* prev = NULL, DDL* next = NULL);
    Result SetDDL(QString, CompleteDegree, QString, QString, QString, QString, DDL*, DDL*); // 设置DDL
    Result SetName(const QString&);                         //设置任务名称
    Result SetCompleteDegree(const CompleteDegree&);        // 设置任务完成度
    Result SetDuration(const QString&);                     // 设置任务持续时间
    Result SetCommence(const QString&);                     // 设置任务开始时间
    Result SetDue(const QString&);                          // 设置任务截止时间
    Result AddDescription(const QString&);                  // 添加DDL描述
    Result DeleteDescription(const int&);                   //删除某描述
    Result ModifyDescription(const int&, const QString&);   //修改某描述
    Result SetPrev(const DDL*);                             // 设置前驱任务
    Result SetNext(DDL*);                                   // 设置后继任务

    double GetUrgency();                  // 返回DDL紧迫程度，预期耗时/(DDL时间-当前时间)*100%
    DDL* GetPrev();                       // 返回前驱任务
    DDL* GetNext();                       // 返回后继任务
    bool operator < (const DDL&);         // 比较时间的先后用于排序："<"——先发生
    bool operator > (const DDL&);         // 比较时间的先后用于排序：">"——后发生

    // 常引用作为私有变量的公有只读版本
    const QString& Name;
    const CompleteDegree& CompleteState;
    const vector<Description>& AllDescription;
    const QTime& Duration;
    const QDateTime& Commence;
    const QDateTime& Due;

private:
    QString m_name;                     //任务名称
    CompleteDegree m_completeDegree;    // 任务完成度
    QTime m_duration;                   // 任务持续时间
    QDateTime m_commence;               //任务开始时间
    QDateTime m_due;                    // 任务截止时间
    Description m_description;          //任务描述
    DDL* m_prev;                        // 前驱任务
    DDL* m_next;                        // 后继任务
    vector<Description> m_AllDescription;


    static vector<shared_ptr<DDL>> m_AllDDL;
// 说明：任务结束时间在基类QDateTime中，任务紧迫度通过接口GetUrgency()间接计算，不用属性进行维护
};

#endif // DDL_H
