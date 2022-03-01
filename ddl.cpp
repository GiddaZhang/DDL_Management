#include "ddl.h"
#include <description.h>

vector<shared_ptr<DDL>> DDL::m_allDDL{};// 静态存储DDL对象

DDL::DDL(QString name, QString commence, QString due, QString description,
          QString filePath,int est_Day, float est_Hour, QString prev, QString next):
    Name(m_name),CompleteState(m_completeState), Duration(m_duration_sec),
    Estimation(m_estimation_sec), Commence(m_commence), Due(m_due),
    TimeToStart(m_timeToStart), Prev(m_prev), Next(m_next),
    AllDescription(m_allDescription), FilePath(m_filePath), m_filePath(filePath){
    //因为无法在调用构造函数的情况下验证数据合理性，所以这里先假设业务流程类已经先调用了静态验证函数
    SetName(name);
    SetCompleteDegree(TO_BE_STARTED);
    SetCommence_Con(commence);
    SetDue_Con(due);
    SetEstimation_Con(est_Day, est_Hour);
    SetPrev(prev);
    SetNext(next);
    AddDescription(description);
    SetDuration();
    SetTimeToStart();
}

Set_Result DDL::SetName(const QString& name){
    //在m_allDDL中寻找重名者
    auto Finder = [&name](shared_ptr<DDL> ptr)->bool{return (ptr->Name == name);};
    auto it = find_if(m_allDDL.begin(), m_allDDL.end(), Finder);
    if (it == m_allDDL.end()) {//若未找到则为合理名字
        m_name = name;
        return VALID;
    }else{
        return INVALID;
    }
}

Set_Result DDL::SetCompleteDegree(const CompleteDegree& degree){
    m_completeState = degree;
    return VALID;
}

Set_Result DDL::SetCommence(const QString& commence){
    //将开始时间与此时相比，只有晚于此时且早于默认上限才合理
    QDateTime Temp = QDateTime::fromString(commence, "yyyy-MM-dd hh:mm:ss");
    if(Temp >= QDateTime::currentDateTime()
     && Temp <= QDateTime::fromString("2050-01-01 00:00:00")){
        m_commence = Temp;
        SetDuration();
        return VALID;
    }else{
        return INVALID;
    }
}

Set_Result DDL::SetDue(const QString& due){
    //将开始时间与开始时间相比，只有晚于开始时间且早于默认上限才合理
    QDateTime Temp = QDateTime::fromString(due, "yyyy-MM-dd hh:mm:ss");
    if(Temp>m_commence && Temp <= QDateTime::fromString("2050-01-01 00:00:00")){
        m_due = Temp;
        SetDuration();
        return VALID;
    }else{
        return INVALID;
    }
}

Set_Result DDL::SetEstimation(const int& est_Day, const float& est_Hour){
    //截止时间向前推预估时间，不超过发布时间才合理
    qint64 Temp_Sec = (qint64)(est_Day * 24 + est_Hour) * 3600;
    QDateTime Temp = m_due.addSecs(- Temp_Sec);
    if(Temp > m_commence){
        m_estimation_sec = Temp_Sec;
        SetTimeToStart();
        return VALID;
    }else{
        return INVALID;
    }
}

Set_Result DDL::SetPrev(QString prev){
    if(prev.localeAwareCompare("UNKNOWN") == 0){//OK：并不存在前驱
        m_prev = prev;
        return VALID;
    }
    //在m_allDDL中搜索prev
    auto Finder = [&prev](shared_ptr<DDL> ptr)->bool{return (ptr->Name == prev);};
    auto it = find_if(m_allDDL.begin(), m_allDDL.end(), Finder);
    if (it == m_allDDL.end()) {//若未找到则前驱不存在
        return INVALID;
    }else{
        m_prev = prev;
        return VALID;
    }
}

Set_Result DDL::SetNext(QString next){
    if(next == "UNKNOWN"){//OK：并不存在后继
        m_next = next;
        return VALID;
    }else if(next.localeAwareCompare(m_prev) == 0){//Error：前驱后继相同
        return INVALID;
    }
    //在m_allDDL中搜索next
    auto Finder = [&next](shared_ptr<DDL> ptr)->bool{return (ptr->Name == next);};
    auto it = find_if(m_allDDL.begin(), m_allDDL.end(), Finder);
    if (it == m_allDDL.end()) {//若未找到则后继不存在
        return INVALID;
    }else{
        m_next = next;
        return VALID;
    }
}

Set_Result DDL::SetDuration(){
    m_duration_sec = m_commence.secsTo(m_due);
    return VALID;
}

Set_Result DDL::SetTimeToStart(){
    m_timeToStart = m_due.addSecs(- m_estimation_sec);
    return VALID;
}

Set_Result DDL::SetCommence_Con(const QString& commence){
    //将开始时间与此时相比，只有晚于此时且早于默认上限才合理
    QDateTime Temp = QDateTime::fromString(commence, "yyyy-MM-dd hh:mm:ss");
    if(Temp >= QDateTime::currentDateTime()
     && Temp <= QDateTime::fromString("2050-01-01 00:00:00")){
        m_commence = Temp;
        return VALID;
    }else{
        return INVALID;
    }
}

Set_Result DDL::SetDue_Con(const QString& due){
    //将开始时间与开始时间相比，只有晚于开始时间且早于默认上限才合理
    QDateTime Temp = QDateTime::fromString(due, "yyyy-MM-dd hh:mm:ss");
    if(Temp>m_commence && Temp <= QDateTime::fromString("2050-01-01 00:00:00")){
        m_due = Temp;
        return VALID;
    }else{
        return INVALID;
    }
}

Set_Result DDL::SetEstimation_Con(const int& est_Day, const float& est_Hour){
    //截止时间向前推预估时间，不超过发布时间才合理
    qint64 Temp_Sec = (qint64)(est_Day * 24 + est_Hour) * 3600;
    QDateTime Temp = m_due.addSecs(- Temp_Sec);
    if(Temp > m_commence){
        m_estimation_sec = Temp_Sec;
        return VALID;
    }else{
        return INVALID;
    }
}

Set_Result DDL::AddDescription(const QString& description){
    Description Temp = (description);
    m_allDescription.push_back(Temp);
    return VALID;
}

Set_Result DDL::DeleteDescription(const int& num){
    //若超出范围，不合理
    if(num < 0 || num >= (int)m_allDescription.size()){
        return INVALID;
    }else{
        auto it = m_allDescription.begin() + num;
        m_allDescription.erase(it);
    }
}

Set_Result DDL::ModifyDescription(const int& num, const QString& new_Description){
    //若超出范围，不合理
    if(num < 0 || num >= (int)m_allDescription.size()){
        return INVALID;
    }else{
        auto it = m_allDescription.begin() + num;
        it->Change(new_Description);

    }
}
Set_Result AddPath(const QString& filePath){
    //待完成
}

Set_Result DeletePath(const int& num){
    //待完成
}

Set_Result ModifyPath(const int& num, const QString& filePath){
    //待完成
}

double DDL::GetUrgency(){
    qint64 cur_sec = QDateTime::currentDateTime().secsTo(this->Due);    // 目前剩余时间，单位秒
    return ((double) m_duration_sec / (double)cur_sec) * 100;
}

bool DDL::operator < (const DDL& b)
{
    return this->m_due < b.Due;
}

bool DDL::operator > (const DDL& b)
{
    return this->m_due > b.Due;
}

void DDL::LoadFromFile(const QString& fileName){
    //待完成
}

void DDL::SaveToFile(const QString& fileName){
    //待完成
}

Set_Result DDL::Verify(QString name, QString commence, QString due,
              int est_Day, float est_Hour, QString prev, QString next){
    auto Finder1 = [&name](shared_ptr<DDL> ptr)->bool{return (ptr->Name == name);};
    auto it1 = find_if(m_allDDL.begin(), m_allDDL.end(), Finder1);
    if (it1 != m_allDDL.end()) {//若未找到则为合理名字
        return INVALID_NAME;
    }

    QDateTime Temp1 = QDateTime::fromString(commence, "yyyy-MM-dd hh:mm:ss");
    if(Temp1 < QDateTime::currentDateTime()
     || Temp1 > QDateTime::fromString("2050-01-01 00:00:00")){
        return INVALID_COMMENCE;
    }

    QDateTime Temp2 = QDateTime::fromString(due, "yyyy-MM-dd hh:mm:ss");
    if(Temp2 <= Commence || Temp2 > QDateTime::fromString("2050-01-01 00:00:00")){
        return INVALID_DUE;
    }

    qint64 Temp_Sec = (qint64)(est_Day * 24 + est_Hour) * 3600;
    QDateTime Temp3 = Due.addSecs(- Temp_Sec);
    if(Temp3 <= Commence){
        return INVALID_ESTIMATE;
    }

    auto Finder2 = [&prev](shared_ptr<DDL> ptr)->bool{return (ptr->Name == prev);};
    auto it2 = find_if(m_allDDL.begin(), m_allDDL.end(), Finder2);
    if (it2 == m_allDDL.end()) {//若未找到则前驱不存在
        return INVALID_PREV;
    }

    if(next.localeAwareCompare(Prev) == 0){//Error：前驱后继相同
        return INVALID_NEXT;
    }
    auto Finder3 = [&next](shared_ptr<DDL> ptr)->bool{return (ptr->Name == next);};
    auto it3 = find_if(m_allDDL.begin(), m_allDDL.end(), Finder3);
    if (it3 == m_allDDL.end()) {//若未找到则后继不存在
        return INVALID_NEXT;
    }
    return VALID;
}
