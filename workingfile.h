#ifndef WORKINGFILE_H
#define WORKINGFILE_H

#include <QString>
#include <memory>
#include <vector>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QDate>
#include <QTime>
#include <QFile>
using namespace std;

// 判断文件操作是否合理
enum class Result
{
    SUCCESS,
    NFOUND,
    FAILURE,
    AlreadyExist
};

class WorkingFile
{
public:
    WorkingFile(const QString&);          // 构造函数
    void AddFile(const QString&);         // 新建文件链接
    Result OpenFile(const QString&);      // 使用默认打开方式打开文件链接
    Result SaveToFolder(const QString&);  // 备份文件链接

    // 常引用作为私有变量的公有只读版本
    const QString& FilePath;
private:
    // 这里需要管理员注意：windows默认的路径以\作分隔符，而qt则需要/分隔，可能需要做一步转换。
    QString m_filePath;
};

bool isDirExist(const QString&);// 检测路径是否存在
QString GetCurrentPath();       // 获取当前文件夹路径


#endif // WORKINGFILE_H
