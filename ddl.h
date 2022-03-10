/****************************************************************************************************************************
【类名】             DDL
【功能】             底层类，程序处理的最基本模块
【接口说明】          CompleteDegree：用于为用户提供可以描述当前进展的选项
                    Set_Result：Set函数返回值，判断修改是否合理

                    成员：任务名称、任务完成度、任务发布时间、任务截止时间、任务活跃期总时长、任务预期完成耗时、任务最晚开始时刻
                    任务前驱、任务后继、任务描述序列、任务相关文档路径序列、存储文档路径的Workpath对象

                    公有Set接口：修改名称、完成度、发布日期时间、截止日期时间、前驱任务、后继任务、预估花费时长

                    内部Set接口：修改任务发布-截止总时长、最晚开始工作时间（随着前三个时间参数的变化，此函数自动被调用）
                    静态公有接口：（程序开始时）DDL信息读取与DDL对象构造、（程序结束是）DDL信息写入、产生对象前验证数据合理性
                    其他公有接口：增、删、改任务描述、增、删、改文档路径、获取任务紧迫度

                    NOTE:commence和due的set函数可能存在问题
****************************************************************************************************************************/
#ifndef DDL_H
#define DDL_H

#include <QString>
#include <QDateTime>
#include <QTime>
#include <memory>
#include <vector>
#include <QDebug>
#include <iostream>
#include <QFileInfo>
#include "description.h"
#include "workingfile.h"
using namespace std;

//完成情况，为用户提供几个可选选项
enum CompleteDegree{
    TO_BE_STARTED,STARTED,QUARTER,HALF_WAY,THREE_QUARTERS,TO_BE_REFINED,COMPLETED,HANDED_IN
};

//修改操作是否合理
enum Set_Result{
    VALID,INVALID, INVALID_NAME, INVALID_COMMENCE, INVALID_DUE,
    INVALID_ESTIMATE, INVALID_PREV, INVALID_NEXT
};

//从文件读写操作是否合理
enum Read_Write_Result{
    FAIL_TO_OPEN, EMPTY, INVALID_DATETIME, SUCCESS
};

class DDL{

public:
    // 构造函数
    DDL(QString name = "UNKNOWN", QString commence = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),
        QString due = "2050-01-01 00:00:00", QString description = "PLAIN", QString filePath = "NULL",
        int est_Day = 0, float est_Hour = 0.0, QString prev = "PREV", QString next = "Next");

    // 公有Setter
    Set_Result SetName(const QString&);                         // 设置任务名称
    Set_Result SetCompleteDegree(const CompleteDegree&);        // 设置任务完成度
    Set_Result SetCommence(const QString&);                     // 设置任务开始时间
    Set_Result SetDue(const QString&);                          // 设置任务截止时间
    Set_Result SetEstimation(const int&, const float&);         // 设置预期完成耗时
    Set_Result SetPrev(QString);                                // 设置前驱任务
    Set_Result SetNext(QString);                                // 设置后继任务

    // 描述语&文档路径的增删改操作
    Set_Result AddDescription(const QString&);                  // 添加DDL描述，时间为当前时间
    Set_Result AddDescription(const QString&, const QDateTime&);// 添加DDL描述，时间由第二个参数给定
    Set_Result DeleteDescription(const int&);                   // 删除描述
    Set_Result ModifyDescription(const int&, const QString&);   // 修改描述
    Set_Result AddPath(const QString&);                         // 添加文档路径
    Set_Result DeletePath(const int&);                          // 删除文档路径
    Set_Result ModifyPath(const int&, const QString&);          // 修改文档路径

    // 打开所有文件
    FileResult OpenAllFile();                                   // 一口气打开所有文件

    // 公有Getter
    double GetUrgency();                                        // 返回DDL紧迫程度，预期耗时/(DDL时间-当前时间)*100%
    QString GetName() const;                                    // 返回DDL名称
    QDateTime GetDue() const;                                   // 返回DDL截止时间
    QString GetNext() const;                                    // 返回后继DDL名称
    QString GetPrev() const;                                    // 返回前驱DDL名称
    vector<WorkingFile> GetWorkingFile() const;                 // 返回Workingfile向量

    // 重载运算符
    bool operator < (const DDL&);                               // 比较时间的先后用于排序："<"——先发生
    bool operator > (const DDL&);                               // 比较时间的先后用于排序：">"——后发生

    //构造前验证（供业务流程调用）
    Set_Result Verify(QString, QString, QString, int, float, QString, QString);//构造对象前验证数据合理性

    //静态公有接口
    static Read_Write_Result LoadFromFile();                    // 加载信息，路径由函数自动确定
    static Read_Write_Result SaveToFile();                      // 存储信息，路径由函数自动确定
    static shared_ptr<DDL> GetDDLPtr(const QString&);           // 获得全部DDL

    void OutputToStream(ostream&) const;                        // 输出流

    // 公有常引用作为私有变量的只读版本
//    const QString& Name;
//    const CompleteDegree& CompleteState;
//    const qint64& Duration;
//    const qint64& Estimation;
//    const QDateTime& Commence;
//    const QDateTime& Due;
//    const QDateTime& TimeToStart;
//    const QString& Prev;
//    const QString& Next;
//    const vector<Description>& AllDescription;
//    const vector<WorkingFile>& AllFilePath;

protected:
    Set_Result SetDuration();                                   // 设置任务持续时间
    Set_Result SetTimeToStart();                                // 设置最晚开始时间
    Set_Result SetCommence_Con(const QString&);                 // 设置任务开始时间，仅用于构造函数中
    Set_Result SetDue_Con(const QString&);                      // 设置任务截止时间，仅用于构造函数中
    Set_Result SetEstimation_Con(const int&, const float&);     // 设置预期完成耗时，仅用于构造函数中

private:
    QString m_name;                                             // 任务名称
    CompleteDegree m_completeState;                             // 任务完成度
    qint64 m_duration_sec;                                      // 任务持续时间（秒）
    qint64 m_estimation_sec;                                    // 预期耗时
    QDateTime m_commence;                                       // 任务开始时间
    QDateTime m_due;                                            // 任务截止时间
    QDateTime m_timeToStart;                                    // 最晚开始时间
    QString m_prev;                                             // 前驱任务
    QString m_next;                                             // 后继任务
    vector<Description> m_allDescription;           // DDL描述语序列
    vector<WorkingFile> m_allFilePath;              // 文档路径序列

    static vector<shared_ptr<DDL>> m_allDDL;                    // 静态存储DDL对象
// 说明：任务结束时间在基类QDateTime中，任务紧迫度通过接口GetUrgency()间接计算，不用属性进行维护
};

CompleteDegree fromStr(const string&);                          // 从string转换到CompleteDegree
string toStr(const CompleteDegree&);                            // 从string转换到CompleteDegree

#endif // DDL_H
