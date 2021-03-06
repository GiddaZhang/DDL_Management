#include "ddl.h"
#include "description.h"
#include <fstream>

vector<shared_ptr<DDL>> DDL::m_allDDL{};// 静态存储DDL对象

DDL::DDL(QString name, QString commence, QString due, QString description,
          QString filePath,int est_Day, float est_Hour, QString prev, QString next){
    //因为无法在调用构造函数的情况下验证数据合理性，所以这里先假设业务流程类已经先调用了静态验证函数
    SetName(name);
    SetCompleteDegree(TO_BE_STARTED);
    SetCommence_Con(commence);
    SetDue_Con(due);
    SetEstimation_Con(est_Day, est_Hour);
    SetPrev(prev);
    SetNext(next);
    AddDescription(description);
    AddPath(filePath);
    SetDuration();
    SetTimeToStart();

    if(!this->isDDLexsisted(name) && name != "UNKNOWN")
        // 不往里加默认构造的，也不往里加已经有的
        m_allDDL.push_back(shared_ptr<DDL>(this));
}

DDL::DDL(const DDL& other)
{
    this->m_name = other.m_name;
    this->m_completeState = other.m_completeState;
    this->m_duration_sec = other.m_duration_sec;
    this->m_estimation_sec = other.m_estimation_sec;
    this->m_commence = other.m_commence;
    this->m_due = other.m_due;
    this->m_timeToStart = other.m_timeToStart;
    this->m_prev = other.m_prev;
    this->m_next = other.m_next;
    this->m_allDescription = other.m_allDescription;
    this->m_allFilePath = other.m_allFilePath;
}

Set_Result DDL::SetDDL(QString name, QString commence, QString due, QString description, QString filePath,
                  int est_Day, float est_Hour, QString prev, QString next){
    SetName(name);
    SetCompleteDegree(TO_BE_STARTED);
    SetCommence_Con(commence);
    SetDue_Con(due);
    SetEstimation_Con(est_Day, est_Hour);
    SetPrev(prev);
    SetNext(next);
    AddDescription(description);
    AddPath(filePath);
    SetDuration();
    SetTimeToStart();
}

Set_Result DDL::SetName(const QString& name, bool hard){
    //在m_allDDL中寻找重名者
    auto Finder = [&name](shared_ptr<DDL> ptr)->bool{return (ptr->GetName() == name);};
    auto it = find_if(m_allDDL.begin(), m_allDDL.end(), Finder);
    if(!hard) {
        // 此时不允许重名
        if (it == m_allDDL.end()) {//若未找到则为合理名字
            m_name = name;
            return VALID;
        }else{
            return INVALID;
        }
    }
    else {
        m_name = name;
    }
}

Set_Result DDL::SetCompleteDegree(const CompleteDegree& degree){
    m_completeState = degree;
    return VALID;
}

Set_Result DDL::SetCommence(const QString& commence){
    //将开始时间与此时相比，只有晚于此时且早于默认上限才合理
    QDateTime Temp = QDateTime::fromString(commence, "yyyy-MM-dd hh:mm:ss");
    if(Temp >= QDateTime::currentDateTime())
        //&& Temp <= QDateTime::fromString("2050-01-01 00:00:00")加上这个会有问题
    {
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
    if(Temp > m_commence){
        // 这里加上这个会赋不上值：&& Temp <= QDateTime::fromString("2050-01-01 00:00:00")
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
//    if(prev.localeAwareCompare("UNKNOWN") == 0){//OK：并不存在前驱
//        m_prev = prev;
//        return VALID;
//    }
//    //在m_allDDL中搜索prev
//    auto Finder = [&prev](shared_ptr<DDL> ptr)->bool{return (ptr->GetName() == prev);};
//    auto it = find_if(m_allDDL.begin(), m_allDDL.end(), Finder);
//    if (it == m_allDDL.end()) {//若未找到则前驱不存在
//        return INVALID;
//    }else{
//        m_prev = prev;
//        return VALID;
//    }
    m_prev = prev;
    return VALID;
}

Set_Result DDL::SetNext(QString next){
//    if(next == "UNKNOWN"){//OK：并不存在后继
//        m_next = next;
//        return VALID;
//    }else if(next.localeAwareCompare(m_prev) == 0){//Error：前驱后继相同
//        return INVALID;
//    }
//    //在m_allDDL中搜索next
//    auto Finder = [&next](shared_ptr<DDL> ptr)->bool{return (ptr->GetName() == next);};
//    auto it = find_if(m_allDDL.begin(), m_allDDL.end(), Finder);
//    if (it == m_allDDL.end()) {//若未找到则后继不存在
//        return INVALID;
//    }else{
//        m_next = next;
//        return VALID;
//    }
    m_next = next;
    return VALID;
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
    if(Temp >= QDateTime::currentDateTime().addSecs(-2) &&
       Temp <= QDateTime::fromString("2050-01-01 00:00:00", "yyyy-MM-dd hh:mm:ss")){
        m_commence = Temp;
        return VALID;
        // if语句中之所以要>=当前时间-2s，是因为默认构造函数中commence是当前时间，但
        // 函数传到这里需要时间，有一个时间差，会导致commence晚于这里的currentDateTime
    }else{
        return INVALID;
    }
}

Set_Result DDL::SetDue_Con(const QString& due){
    //将开始时间与开始时间相比，只有晚于开始时间且早于默认上限才合理
    QDateTime Temp = QDateTime::fromString(due, "yyyy-MM-dd hh:mm:ss");
    if(Temp > m_commence){
        // 加这个有问题Temp <= QDateTime::fromString("2050-01-01 00:00:00", "yyyy-MM-dd hh:mm:ss"
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
    if(description == "PLAIN")
        return VALID;
    Description Temp(description);
    m_allDescription.push_back(Temp);
    return VALID;
}

Set_Result DDL::AddDescription(const QString& description, const QDateTime& time){
    Description Temp(description, time);
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
        return VALID;
    }
}

Set_Result DDL::DeleteDescription(const QString& note)
{
    for(int i = 0; i < m_allDescription.size(); i++) {
        if(m_allDescription[i].GetNote() == note) {
            return DeleteDescription(i);
        }
    }
}

Set_Result DDL::ModifyDescription(const int& num, const QString& new_Description){
    //若超出范围，不合理
    if(num < 0 || num >= (int)m_allDescription.size()){
        return INVALID;
    }else{
        auto it = m_allDescription.begin() + num;
        it->Change(new_Description);
        return VALID;
    }
}
Set_Result DDL::AddPath(const QString& filePath){
    // 先判断路径是否存在
    QFileInfo dir(filePath);
    if(dir.exists() == false) {
        return INVALID;
    }
    WorkingFile Temp(filePath);
    m_allFilePath.push_back(Temp);
    return VALID;
}

Set_Result DDL::DeletePath(const int& num){
    //若超出范围，不合理
    if(num < 0 || num >= (int)m_allFilePath.size()){
        return INVALID;
    }else{
        auto it = m_allFilePath.begin() + num;
        m_allFilePath.erase(it);
        return VALID;
    }
}

Set_Result DDL::DeletePath(const QString& path)
{
    for(int i = 0; i < m_allFilePath.size(); i++) {
        if(m_allFilePath[i].GetFilePath() == path) {
            return DeletePath(i);
        }
    }
}

Set_Result DDL::ModifyPath(const int& num, const QString& new_FilePath){
    //若超出范围，不合理
    if(num < 0 || num >= (int)m_allFilePath.size()){
        return INVALID;
    }else{
        auto it = m_allFilePath.begin() + num;
        if(it->Change(new_FilePath) == FileResult::SUCCESS) {
            return VALID;
        }
        return INVALID;
    }
}

FileResult DDL::OpenAllFile()
{
    for(auto it = m_allFilePath.begin();it != m_allFilePath.end();it++) {
        if(it->OpenFile() != FileResult::SUCCESS) {
            return FileResult::FAILURE;
        }
    }
    return FileResult::SUCCESS;
}

FileResult DDL::OpenFile(const QString& path)
{
    for(auto it = m_allFilePath.begin();it != m_allFilePath.end();it++) {
        if(it->GetFilePath() == path) {
            if(it->OpenFile() == FileResult::SUCCESS)
                return FileResult::SUCCESS;
        }
    }
    return FileResult::NFOUND;
}

double DDL::GetUrgency()
{
    qint64 cur_sec = QDateTime::currentDateTime().secsTo(this->m_due);    // 目前剩余时间，单位秒
    return ((double) m_duration_sec / (double)cur_sec) * 100;
}

QString DDL::GetName() const
{
    return this->m_name;
}

QDateTime DDL::GetComm() const
{
    return this->m_commence;
}

QDateTime DDL::GetDue() const
{
    return this->m_due;
}

QString DDL::GetNext() const
{
    return this->m_next;
}

QString DDL::GetPrev() const
{
    return this->m_prev;
}

vector<WorkingFile> DDL::GetWorkingFile() const
{
    return this->m_allFilePath;
}

shared_ptr<DDL> DDL::GetDDLPtr(const QString& name)
{
    for(auto it = m_allDDL.begin();it != m_allDDL.end();it++){
        if((*it)->GetName() == name){
            return *it;
        }
    }
    return nullptr;
}

vector<shared_ptr<DDL>> DDL::GetAllDDLPtr()
{
    return m_allDDL;
}

vector<shared_ptr<DDL> > *DDL::GetAllDDLPtrPtr()
{
    return &m_allDDL;
}

void DDL::RemoveDDL(const QString& name)
{
    auto Finder = [&name](shared_ptr<DDL> ptr)->bool{return (ptr->GetName() == name);};
    auto it = find_if(m_allDDL.begin(), m_allDDL.end(), Finder);
    //判断DDL是否存在
    if (it == m_allDDL.end()) {
        throw invalid_argument("No such DDL.");
    }
    //存在则直接erase
    else {
        it = m_allDDL.erase(it);
    }
}

bool DDL::isDDLexsisted(const QString& name)
{
    for(auto it = m_allDDL.begin();it != m_allDDL.end();it++){
        if((*it)->GetName() == name){
            return true;
        }
    }
    return false;
}

bool DDL::operator < (const DDL& b)
{
    return this->m_due < b.GetDue();
}

bool DDL::operator > (const DDL& b)
{
    return this->m_due > b.GetDue();
}

Read_Write_Result DDL::LoadFromFile()
{
    // 确定user文件路径
    QString current_path = GetCurrentPath();
    QString filePath = current_path + "/user/user.txt";

    ifstream File(filePath.toStdString());    // 定义一个输入流对象
    //上面是Debug or Realease下打开文件。如果打开失败，则是发布状态，用另一方式打开
    if (!File.is_open()) {
        current_path = QDir::currentPath();
        filePath = current_path + "/user/user.txt";
        File.open(filePath.toStdString());
        if (!File.is_open()) {
            return Read_Write_Result::FAIL_TO_OPEN;
        }
    }

    unsigned long DDLCount;        // DDL数量
    File >> DDLCount;              // 从第一行读取数量
    File.get();                    // 读取第一行剩下的空格

    if (DDLCount == 0) {
        return Read_Write_Result::EMPTY;// 如果没有DDL，直接返回空文件错误
    }

    // 否则逐个读取
    string f_name;
    string f_complete_degree_str;
    string f_commence_str;
    string f_due_str;
    string f_prev;
    string f_next;
    string f_timestamp;
    string f_note;
    string f_path;
    string HOLDER;
    int f_est_day;
    float f_est_Hour;

    vector<Description> f_allDescrip;
    vector<WorkingFile> f_allFilePath;

    for(unsigned long i = 0; i < DDLCount; i++) {

        // 先读前八行
        getline(File, f_name);
        File >> f_complete_degree_str;
//        File >> f_name >> f_complete_degree_str;
        // 名字可能有空格，不能直接 >>，得getline
        File.get();
        getline(File, f_commence_str);
        getline(File, f_due_str);
        File >> f_prev >> f_next >> f_est_day >> f_est_Hour;
        File.get();

        // 然后读description的2n行
        while(HOLDER != "<DEND>") {
            getline(File, HOLDER);      // 还没读完，先试读一行

            if(HOLDER != "<DEND>") {
                f_note = HOLDER;        // 不是结尾，还给f_note
                getline(File, f_timestamp);

                // 类型转换
                QString f_note_qstr = QString::fromStdString(f_note);
                QString f_timestamp_qstr = QString::fromStdString(f_timestamp);
                QDateTime f_timestamp_datetime = QDateTime::fromString(f_timestamp_qstr, "yyyy-MM-dd hh:mm:ss");
                // 整进向量
                Description Temp_1(f_note_qstr, f_timestamp_datetime);
                f_allDescrip.push_back(Temp_1);
            }
        }

        // 然后读filepath的m行
        while(HOLDER != "<WEND>") {
            getline(File, HOLDER);      // 还没读完，先试读一行

            if(HOLDER != "<WEND>") {
                f_path = HOLDER;        // 不是结尾，还给f_path

                // 类型转换
                QString f_path_qstr = QString::fromStdString(f_path);
                // 整进向量里
                WorkingFile Temp_2(f_path_qstr);
                f_allFilePath.push_back(Temp_2);
            }
        }

        DDL* ddl = new DDL(QString::fromStdString(f_name));       // 调用默认构造函数
        ddl->SetCompleteDegree(fromStr(f_complete_degree_str));
        ddl->SetCommence(QString::fromStdString(f_commence_str));
        ddl->SetDue(QString::fromStdString(f_due_str));
        ddl->SetEstimation(f_est_day, f_est_Hour);
        ddl->SetPrev(QString::fromStdString(f_prev));
        ddl->SetNext(QString::fromStdString(f_next));

        ddl->DeleteDescription(0);      // 删除PLAIN
        ddl->DeletePath(0);             // 删除NULL

        // 添加描述信息
        for(auto it = f_allDescrip.begin(); it != f_allDescrip.end(); it ++) {
            ddl->AddDescription(it->GetNote(), it->GetStamp());
        }
        // 添加文件路径信息
        for(auto it = f_allFilePath.begin(); it != f_allFilePath.end(); it ++) {
            ddl->AddPath(it->GetFilePath());
        }

        // 每个DDL读完后，清空临时向量
        f_allDescrip.clear();
        f_allFilePath.clear();
    }

    File.close();//关闭文件
    return Read_Write_Result::SUCCESS;
}

Read_Write_Result DDL::SaveToFile()
{
    // 确定user文件路径
    QString current_path = GetCurrentPath();
    QString filePath = current_path + "/user/user.txt";

    ofstream File(filePath.toStdString());// 定义一个输出流对象
    //上面是Debug or Realease下打开文件。如果打开失败，则是发布状态，用另一方式打开
    if (!File.is_open()) {
        current_path = QDir::currentPath();
        filePath = current_path + "/user/user.txt";
        File.open(filePath.toStdString());
        if (!File.is_open()) {
            return Read_Write_Result::FAIL_TO_OPEN;
        }
    }
    auto Saver = [&File](shared_ptr<DDL> Ptr) {Ptr->OutputToStream(File); };
    File << m_allDDL.size() << endl;                        // 首先输入DDL数量
    for_each(m_allDDL.begin(), m_allDDL.end(), Saver);      // 再一个接一个把DDL列表里的信息输入文件中
    File.close();//关闭文件
    return Read_Write_Result::SUCCESS;
}

void DDL::OutputToStream(ostream& Stream) const
{
    // 先写前八行
    // 把m_estimation_sec转换成天和小时
    int write_day = m_estimation_sec / 3600 / 24;
    float write_hour = float(m_estimation_sec - write_day * 3600 * 24) / float(3600);
    Stream << m_name.toStdString() << endl
           << toStr(m_completeState) << endl
           << m_commence.toString("yyyy-MM-dd hh:mm:ss").toStdString() << endl
           << m_due.toString("yyyy-MM-dd hh:mm:ss").toStdString() << endl
           << m_prev.toStdString() << endl
           << m_next.toStdString() << endl
           << write_day << endl
           << write_hour << endl;

    // 再写Description
    for (auto it = m_allDescription.begin(); it != m_allDescription.end(); it++) {
        // 先描述
        Description tmp = *it;
        Stream << tmp.GetNote().toStdString() << endl;
        // 再时间
        Stream << tmp.GetStamp().toString("yyyy-MM-dd hh:mm:ss").toStdString() << endl;
    }
    Stream << "<DEND>" << endl;         // 结束符

    // 再写WorkingFile
    for (auto it = m_allFilePath.begin(); it != m_allFilePath.end(); it++) {
        WorkingFile tmp = *it;
        Stream << tmp.GetFilePath().toStdString() << endl;
    }
    Stream << "<WEND>" << endl;         // 结束符
}

Set_Result DDL::Verify(QString name, QString commence, QString due,
              int est_Day, float est_Hour, QString prev, QString next){
    auto Finder1 = [&name](shared_ptr<DDL> ptr)->bool{return (ptr->GetName() == name);};
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
    if(Temp2 <= m_commence || Temp2 > QDateTime::fromString("2050-01-01 00:00:00")){
        return INVALID_DUE;
    }

    qint64 Temp_Sec = (qint64)(est_Day * 24 + est_Hour) * 3600;
    QDateTime Temp3 = m_due.addSecs(- Temp_Sec);
    if(Temp3 <= m_commence){
        return INVALID_ESTIMATE;
    }

    auto Finder2 = [&prev](shared_ptr<DDL> ptr)->bool{return (ptr->GetName() == prev);};
    auto it2 = find_if(m_allDDL.begin(), m_allDDL.end(), Finder2);
    if (it2 == m_allDDL.end()) {//若未找到则前驱不存在
        return INVALID_PREV;
    }

    if(next.localeAwareCompare(m_prev) == 0){//Error：前驱后继相同
        return INVALID_NEXT;
    }
    auto Finder3 = [&next](shared_ptr<DDL> ptr)->bool{return (ptr->GetName() == next);};
    auto it3 = find_if(m_allDDL.begin(), m_allDDL.end(), Finder3);
    if (it3 == m_allDDL.end()) {//若未找到则后继不存在
        return INVALID_NEXT;
    }
    return VALID;
}

CompleteDegree fromStr(const string& in)
{
    // 佛了
    if(in == "TO_BE_STARTED")
        return CompleteDegree::TO_BE_STARTED;
    if(in == "STARTED")
        return CompleteDegree::STARTED;
    if(in == "QUARTER")
        return CompleteDegree::QUARTER;
    if(in == "HALF_WAY")
        return CompleteDegree::HALF_WAY;
    if(in == "THREE_QUARTERS")
        return CompleteDegree::THREE_QUARTERS;
    if(in == "TO_BE_REFINED")
        return CompleteDegree::TO_BE_REFINED;
    if(in == "COMPLETED")
        return CompleteDegree::COMPLETED;
    if(in == "HANDED_IN")
        return CompleteDegree::HANDED_IN;
}

string toStr(const CompleteDegree& in)
{
    // 佛了*2
    if(in == CompleteDegree::TO_BE_STARTED)
        return "TO_BE_STARTED";
    if(in == CompleteDegree::STARTED)
        return "STARTED";
    if(in == CompleteDegree::QUARTER)
        return "QUARTER";
    if(in == CompleteDegree::HALF_WAY)
        return "HALF_WAY";
    if(in == CompleteDegree::THREE_QUARTERS)
        return "THREE_QUARTERS";
    if(in == CompleteDegree::TO_BE_REFINED)
        return "TO_BE_REFINED";
    if(in == CompleteDegree::COMPLETED)
        return "COMPLETED";
    if(in == CompleteDegree::HANDED_IN)
        return "HANDED_IN";
}
